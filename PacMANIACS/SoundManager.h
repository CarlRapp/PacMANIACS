#pragma once
#include "stdafx.h"
#include "Camera.h"
#include <Windows.h>
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

	typedef map<string, IDirectSoundBuffer8*>	MAP_SOUNDBUFFER;
	typedef map<string, IDirectSound3DBuffer8*> MAP_SOUND3DBUFFER;
	typedef map<string, D3DXVECTOR3>			MAP_POSITION;

	MAP_SOUNDBUFFER		gSecondaryBufferMap;
	MAP_SOUND3DBUFFER	gSecondary3DBufferMap;
	MAP_POSITION		gSoundPositionMap;
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