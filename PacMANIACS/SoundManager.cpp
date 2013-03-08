#include "SoundManager.h"


SoundManager::SoundManager(Camera* camera, HWND hwnd)
{
	gCamera = camera;
	gDirectSound = 0;
	gPrimaryBuffer = 0;
	gListener = 0;

	gSoundBufferMap			= MAP_SOUNDBUFFER();
	gSoundBuffer8Map		= MAP_SOUNDBUFFER8();
	gSound3DBuffer8Map		= MAP_SOUND3DBUFFER8();

	bool result;

	result = Initialize(hwnd);
	if(!result)
		::MessageBox(0, "Initializing of the SoundManager failed!", "Error", MB_OK);

}


SoundManager::~SoundManager(void)
{
	gDirectSound = 0;
	gPrimaryBuffer = 0;
	gListener = 0;

	gSoundBuffer8Map.clear();
	gSound3DBuffer8Map.clear();
}

bool SoundManager::Initialize(HWND hwnd)
{
	HRESULT result;

	result = DirectSoundCreate8(NULL, &gDirectSound, NULL);
	if(FAILED(result))
		return false;

	result = gDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if(FAILED(result))
		return false;

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = gDirectSound->CreateSoundBuffer(&bufferDesc, &gPrimaryBuffer, NULL);
	if(FAILED(result))
		return false;

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	result = gPrimaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result))
		return false;

	result = gPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&gListener);
	if(FAILED(result))
		return false;

	gListener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);


	return true;
	
}

bool SoundManager::LoadSoundFile(char* filename)
{
	if (gSoundBuffer8Map.count(filename) > 1)
		return true;

	string path = soundPath + "\\" + filename + ".wav";

	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, path.c_str(), "rb");
	if(error != 0)
		return false;

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1)
		return false;

	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
		return false;

	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
		return false;

	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
		return false;

	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		return false;



	if(waveFileHeader.numChannels != 1)
		return false;

	if(waveFileHeader.sampleRate != 44100)
		return false;

	if(waveFileHeader.bitsPerSample != 16)
		return false;

	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
		return false;

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;


	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;


	result = gDirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if(FAILED(result))
		return false;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData)
		return false;

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize)
		return false;

	error = fclose(filePtr);
	if(error != 0)
		return false;

	result = tempBuffer->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result))
		return false;

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = tempBuffer->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if(FAILED(result))
		return false;

	delete [] waveData;
	waveData = 0;

	gSoundBufferMap.insert(pair<string, IDirectSoundBuffer*>(filename, tempBuffer));
}

string SoundManager::PlaySound(string name, D3DXVECTOR3 position)
{
	return PlaySound(name, position, 0);
}

string SoundManager::LoopSound(string name, D3DXVECTOR3 position)
{
	return PlaySound(name, position, DSBPLAY_LOOPING);
}

string SoundManager::PlaySound(string name, D3DXVECTOR3 position, DWORD dwFlags)
{
	HRESULT result;
	if(gSoundBufferMap.count(name) != 0)
	{
		IDirectSoundBuffer* tempBuffer = 0;
		IDirectSoundBuffer8* tempBuffer8 = 0;
		IDirectSound3DBuffer8* temp3DBuffer8 = 0;

		gDirectSound->DuplicateSoundBuffer(gSoundBufferMap[name], &tempBuffer);

		result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&tempBuffer8);
		if(FAILED(result))
			return "";

		tempBuffer->Release();
		tempBuffer = 0;

		result = tempBuffer8->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&temp3DBuffer8);
		if(FAILED(result))
			return "";
		
		result = tempBuffer8->SetCurrentPosition(0);
		if(FAILED(result))
			return "";

		result = tempBuffer8->SetVolume(DSBVOLUME_MAX);
		if(FAILED(result))
			return "";

		result = temp3DBuffer8->SetPosition(position.x, position.y, position.z, DS3D_IMMEDIATE);
		if(FAILED(result))
			return "";

		result = tempBuffer8->Play(0, 0, dwFlags);
		if(FAILED(result))
			return "";

		string key = GenerateKey(name);

		gSoundBuffer8Map.insert(pair<string, IDirectSoundBuffer8*>(key, tempBuffer8));
		gSound3DBuffer8Map.insert(pair<string, IDirectSound3DBuffer8*>(key, temp3DBuffer8));

		return key;
	}

	return "";
}

void SoundManager::StopSound(string key)
{
	if(gSoundBufferMap.count(key) != 0)
	{
		gSoundBuffer8Map[key]->Stop();
		RemoveSound(key);
	}
}

void SoundManager::RemoveSound(string key)
{
	gSoundBuffer8Map[key]->Release();
	gSound3DBuffer8Map[key]->Release();
	gSoundBuffer8Map.erase(key);
	gSound3DBuffer8Map.erase(key);
}


void SoundManager::Update()
{
	D3DXVECTOR3 pos = gCamera->GetPosition();
	D3DXVECTOR3 forward = gCamera->GetForward();
	D3DXVECTOR3 up = gCamera->GetUp();

	gListener->SetPosition(pos.x, pos.y, pos.z, DS3D_DEFERRED);
	gListener->SetOrientation(forward.x, forward.y,forward.z, up.x, up.y,up.z, DS3D_DEFERRED);

	gListener->CommitDeferredSettings();


	vector<string> keys = vector<string>();
	for(MAP_SOUNDBUFFER8::iterator iterator = gSoundBuffer8Map.begin(); iterator != gSoundBuffer8Map.end(); iterator++) 
	{
		DWORD status = 0;
		iterator->second->GetStatus(&status);
		if (!(status & DSBSTATUS_PLAYING))
		{
			keys.push_back(iterator->first);
		}
	}

	for each (string key in keys)
	{
		RemoveSound(key);
	}
}

void SoundManager::SetSoundPath(string path)
{
	soundPath = path;
}

string SoundManager::GenerateKey(string name)
{
	string key;
	for (int i = 0; i < 1000; i++)
	{
		std::stringstream ss;
		ss << i;
		key = name + ss.str();

		if(gSoundBuffer8Map.count(key) == 0 && gSound3DBuffer8Map.count(key) == 0)
			return key;
	}
	return "";
}