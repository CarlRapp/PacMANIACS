#pragma once
#include "stdafx.h"
#include "Camera.h"
#include <MMSystem.h>
#include <dsound.h>
#include <stdio.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;
class SoundManager
{
private:
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

	Camera*						gCamera;
	IDirectSound8*				gDirectSound;
	IDirectSoundBuffer*			gPrimaryBuffer;
	IDirectSound3DListener8*	gListener;


	vector<IDirectSoundBuffer8*>	gSecondaryBuffers;
	vector<IDirectSound3DBuffer8*>	gSecondary3DBuffers;
	vector<D3DXVECTOR3>				gSoundPositions;
public:
	SoundManager(Camera* camera, HWND hwnd);
	~SoundManager(void);

	bool PlaySound(string name, D3DXVECTOR3 postion);
	void Update();
private:
	bool Initialize(HWND hwnd);
	bool LoadSoundFile(char* filename);
	int ConvertToIndex(string soundName);
};