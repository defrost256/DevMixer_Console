// DevMixer_Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "audioUtils.h"

using namespace std;

void handleError(const char* source, PaError err)
{
	cout << "PortAudio " << source << " Error : " << Pa_GetErrorText(err) << endl;
	Pa_Terminate();
}

int printDeviceInfo(bool detailed) 
{
	int n = Pa_GetDeviceCount();
	const PaDeviceInfo* info;
	if (detailed)
		cout << "IDX\t" << setiosflags(std::ios::left) << setw(80) << "Name" << setw(15) << "Input Channels" << setw(15) << "Output Channels" << setw(15) << "Sample Rate" << endl;
	else
		cout << "IDX\t" << setiosflags(std::ios::left) << setw(80) << "Name" << setw(5) << "I/O" << endl;
	for (int i = 0; i < n; i++)
	{
		info = Pa_GetDeviceInfo(i);
		if (detailed)
			cout << i << "\t" << setiosflags(std::ios::left) << setw(80) << info->name << setw(15) << info->maxInputChannels << setw(15) << info->maxOutputChannels << setw(15) << info->defaultSampleRate << endl;
		else
			cout << i << "\t" << setiosflags(std::ios::left) << setw(80) << info->name << setw(5) << (info->maxInputChannels == 0 ? "O" : info->maxOutputChannels == 0 ? "I" : "I/O") << endl;
	}
	cout << setw(0) << resetiosflags(std::ios::left);
	return n;
}

PaStream* connectDevices(int in, int out, float reliability)
{
	PaStream* ret;
	PaStreamParameters inputParams, outputParams;
	const PaDeviceInfo* inputDevice, *outputDevice;
	inputDevice = Pa_GetDeviceInfo(in);
	outputDevice = Pa_GetDeviceInfo(out);
	if (inputDevice == nullptr || outputDevice == nullptr)
		return nullptr;
	int minChannelCount = min(inputDevice->maxInputChannels, outputDevice->maxOutputChannels);
	inputParams.channelCount = minChannelCount;
	outputParams.channelCount = minChannelCount;
	inputParams.device = in;
	outputParams.device = out;
	int minLatency = max(inputDevice->defaultLowInputLatency, outputDevice->defaultLowOutputLatency);
	int maxLatency = min(inputDevice->defaultHighInputLatency, outputDevice->defaultHighOutputLatency);
	int suggestedLatency = lerpInt(minLatency, maxLatency, reliability);
	inputParams.suggestedLatency = suggestedLatency;
	outputParams.suggestedLatency = suggestedLatency;
	inputParams.sampleFormat = paFloat32;
	outputParams.sampleFormat = paFloat32;
	int sampleRate = min(inputDevice->defaultSampleRate, outputDevice->defaultSampleRate);
	PaError err = Pa_OpenStream(&ret, &inputParams, &outputParams, sampleRate, 255, paNoFlag, nullptr, nullptr);
	if (err != paNoError)
	{
		handleError("Open Stream", err);
		return nullptr;
	}
	return ret;
}

int main()
{
	PaError error = Pa_Initialize();
	if (error != paNoError) {
		handleError("Init", error);
		return -1;
	}
	cout << "PortAudio Init Succesful" << endl;
	cout << "DEVICES : " << endl;
	printDeviceInfo(true);
	Pa_Terminate();
    return 0;
}

