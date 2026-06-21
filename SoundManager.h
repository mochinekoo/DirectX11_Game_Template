#pragma once
#include <xaudio2.h>
#include <string>
#include <vector>

struct WaveData {
	WAVEFORMATEX waveFormat;
	char* soundBuffer;
	DWORD size;

	~WaveData() { free(soundBuffer); }
};

struct SoundData {
	IXAudio2SourceVoice* sourceVoice = nullptr;
	XAUDIO2_VOICE_STATE state = {};
};

namespace SoundManager {
	HRESULT Initialize();
	bool LoadFile(const std::string path, WaveData* outData);
	bool Play(const std::string& fileName, WaveData* data, bool loop);
	void Stop(int index);
	void Release();

}