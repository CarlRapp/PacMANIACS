#pragma once
#include "stdafx.h"
#include "Camera.h"
#include <MMSystem.h>
#include <dsound.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

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

	typedef map<string, IDirectSoundBuffer*>	MAP_SOUNDBUFFER;
	typedef map<string, IDirectSoundBuffer8*>	MAP_SOUNDBUFFER8;
	typedef map<string, IDirectSound3DBuffer8*> MAP_SOUND3DBUFFER8;

	MAP_SOUNDBUFFER		gSoundBufferMap;

	MAP_SOUNDBUFFER8	gSoundBuffer8Map;
	MAP_SOUND3DBUFFER8	gSound3DBuffer8Map;

	string soundPath;

	string	PlaySound(string name, D3DXVECTOR3 postion, DWORD dwFlags);
	void	RemoveSound(string key);
	string GenerateKey(string name);

public:
	SoundManager(Camera* camera, HWND hwnd);
	~SoundManager(void);

	bool	LoadSoundFile(char* filename);
	string	PlaySound(string name, D3DXVECTOR3 postion);
	string	LoopSound(string name, D3DXVECTOR3 postion);
	void	StopSound(string key);
	void	Update();

	void	SetSoundPath(string path);

private:
	bool	Initialize(HWND hwnd);
};