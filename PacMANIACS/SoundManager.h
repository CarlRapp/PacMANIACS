#pragma once
#include "Camera.h"
#include "stdafx.h"

#include <MMSystem.h>
#include <dsound.h>
#include <stdio.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

class SoundManager
{
private :
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};


	Camera* gCamera;
	IDirectSound8* gDirectSound;
	IDirectSoundBuffer* gPrimaryBuffer;
	IDirectSound3DListener8* gListener;

	vector<IDirectSound3DBuffer8*> gSecondary3DBuffers;
	vector<IDirectSoundBuffer8*> gSecondaryBuffers;

private :
	bool InitializeDirectSound(HWND hwnd);
	bool LoadSoundFile(char* filename);
public:
	SoundManager(void);
	~SoundManager(void);

	void PlaySound(string SoundName, D3DXVECTOR3 Position);
	void  Update();
};

