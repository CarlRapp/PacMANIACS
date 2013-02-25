#include "SoundManager.h"


SoundManager::SoundManager(Camera* camera, HWND hwnd)
{
	gDirectSound	 = 0;
	gPrimaryBuffer	 = 0;
	gListener		 = 0;
	gSecondary3DBuffers.clear();
	gSecondaryBuffers.clear();

	gCamera = camera;

	InitializeDirectSound(hwnd);

	LoadSoundFile("../PacMANIACS/Sounds/sound01.wav");
}


SoundManager::~SoundManager(void)
{
	gDirectSound	 = 0;
	gPrimaryBuffer	 = 0;
	gListener		 = 0;
	gSecondary3DBuffers.clear();
	gSecondaryBuffers.clear();
}

bool SoundManager::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;

	result = DirectSoundCreate8(NULL, &gDirectSound, NULL);
	if(FAILED(result))
		return false;

	result = gDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize			= sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags			= DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes	= 0;
	bufferDesc.dwReserved		= 0;
	bufferDesc.lpwfxFormat		= NULL;
	bufferDesc.guid3DAlgorithm	= GUID_NULL;

	result = gDirectSound->CreateSoundBuffer(&bufferDesc, &gPrimaryBuffer, NULL);
	if(FAILED(result))
		return false;


	WAVEFORMATEX waveFormat;
	waveFormat.nSamplesPerSec	= 44100;
	waveFormat.wBitsPerSample	= 16;
	waveFormat.nChannels		= 2;
	waveFormat.nBlockAlign		= (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec	= waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize			= 0;

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
	int error;
	FILE* filePtr;
	UINT count;
	WaveHeaderType waveFileHeader;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	IDirectSoundBuffer8** tempSecondaryBuffer;
	IDirectSound3DBuffer8** tempSecondary3DBuffer;

	error = fopen_s(&filePtr, filename, "rb");
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
	waveFormat.wFormatTag		= WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec	= 44100;
	waveFormat.wBitsPerSample	= 16;
	waveFormat.nChannels		= 1;
	waveFormat.nBlockAlign		= (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec	= waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;


	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize			= sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags			= DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes	= waveFileHeader.dataSize;
	bufferDesc.dwReserved		= 0;
	bufferDesc.lpwfxFormat		= &waveFormat;
	bufferDesc.guid3DAlgorithm	= GUID_NULL;

	result = gDirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if(FAILED(result))
		return false;

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&tempSecondaryBuffer);
	if(FAILED(result))
		return false;

	tempBuffer->Release();
	tempBuffer = 0;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData)
		return false;

	error = fclose(filePtr);
	if(error != 0)
		return false;

	result = (*tempSecondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result))
		return false;

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = (*tempSecondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if(FAILED(result))
		return false;

	delete [] waveData;
	waveData = 0;

	result = (*tempSecondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*tempSecondary3DBuffer);
	if(FAILED(result))
		return false;

	
	
	gSecondaryBuffers.push_back(*tempSecondaryBuffer);
	gSecondary3DBuffers.push_back(*tempSecondary3DBuffer);


	return true;
}

void SoundManager::Update()
{
	D3DXVECTOR3 cameraPosition = gCamera->GetPosition();

	gListener->SetPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z, DS3D_IMMEDIATE);
}

void SoundManager::PlaySound(string soundName, D3DXVECTOR3 position)
{
	HRESULT result;

	int index = ConvertToIndex(soundName);

	result = gSecondaryBuffers[index]->SetCurrentPosition(0);
	if(FAILED(result))
		return;

	result = gSecondaryBuffers[index]->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
		return;

	result = gSecondary3DBuffers[index]->SetPosition(position.x, position.y, position.z, DS3D_IMMEDIATE);
	if(FAILED(result))
		return;

	result = gSecondaryBuffers[index]->Play(0, 0, 0);
	if(FAILED(result))
		return;


}

int SoundManager::ConvertToIndex(string soundName)
{
	if(soundName == "Ghost")
		return 0;

	return 0;
}