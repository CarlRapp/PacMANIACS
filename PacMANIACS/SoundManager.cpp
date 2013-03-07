#include "SoundManager.h"


SoundManager::SoundManager(Camera* camera, HWND hwnd)
{
	gCamera = camera;
	gDirectSound = 0;
	gPrimaryBuffer = 0;
	gListener = 0;

	gSecondaryBufferMap		= MAP_SOUNDBUFFER();
	gSecondary3DBufferMap	= MAP_SOUND3DBUFFER();
	gSoundPositionMap		= MAP_POSITION();

	bool result;

	result = Initialize(hwnd);
	if(!result)
		return;

	result = LoadSoundFile("Sounds/LoginScreenIntro4.wav");
	if(!result)
		return;
}


SoundManager::~SoundManager(void)
{
	gDirectSound = 0;
	gPrimaryBuffer = 0;
	gListener = 0;

	gSecondaryBufferMap.clear();
	gSecondary3DBufferMap.clear();
	gSoundPositionMap.clear();
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
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;


	IDirectSoundBuffer8* tempSecBuffer = 0;
	IDirectSound3DBuffer8* tempSec3DBuffer = 0;
	IDirectSoundBuffer8** tempSecondaryBuffer = &tempSecBuffer;
	IDirectSound3DBuffer8** tempSecondary3DBuffer = &tempSec3DBuffer;
	

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

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*tempSecondaryBuffer);
	if(FAILED(result))
		return false;

	tempBuffer->Release();
	tempBuffer = 0;

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

	gSecondaryBufferMap.insert(pair<string, IDirectSoundBuffer8*>(filename, *tempSecondaryBuffer));
	gSecondary3DBufferMap.insert(pair<string, IDirectSound3DBuffer8*>(filename, *tempSecondary3DBuffer));
	gSoundPositionMap.insert(pair<string, D3DXVECTOR3>(filename, D3DXVECTOR3(0, 0, 0)));

}

bool SoundManager::PlaySound(string name, D3DXVECTOR3 position)
{
	HRESULT result;
	if(gSoundPositionMap.count(name) != 0)
	{
		gSoundPositionMap[name] = position;

		result = gSecondaryBufferMap[name]->SetCurrentPosition(0);
		if(FAILED(result))
			return false;

		result = gSecondaryBufferMap[name]->SetVolume(DSBVOLUME_MAX);
		if(FAILED(result))
			return false;

		Update();

		result = gSecondaryBufferMap[name]->Play(0, 0, 0);
		if(FAILED(result))
			return false;

		return true;
	}

	return false;
}

void SoundManager::Update()
{
	D3DXVECTOR3 cameraPosition = gCamera->GetPosition();
	D3DXMATRIX translation, rotation, worldToCamera;

	D3DXMatrixTranslation(&translation, -cameraPosition.x, -cameraPosition.y, -cameraPosition.z);
	D3DXVECTOR3 cameraDirection = gCamera->GetForward();
	D3DXVECTOR3 startDirection(0,0,1);

	float dot = D3DXVec3Dot(&startDirection, &cameraDirection);

	if (dot != 0)
	{
		float angle = acos(dot);
		if (angle != 0)
		{
			D3DXVECTOR3 cross;
			D3DXVec3Cross(&cross, &startDirection, &cameraDirection);
    
			D3DXMatrixRotationAxis(&rotation, &cross, angle);
			float determinant = D3DXMatrixDeterminant(&rotation);
			D3DXMatrixInverse(&rotation, &determinant, &rotation);
		}
	}

	worldToCamera = translation * rotation;
	
	D3DXVECTOR3 newPosition;
	for(MAP_SOUND3DBUFFER::iterator iterator = gSecondary3DBufferMap.begin(); iterator != gSecondary3DBufferMap.end(); iterator++) 
	{
		D3DXVec3TransformCoord(&newPosition, &gSoundPositionMap[iterator->first], &worldToCamera);
		iterator->second->SetPosition(newPosition.x, newPosition.y, newPosition.z, DS3D_IMMEDIATE);
	}
}

int SoundManager::ConvertToIndex(string soundName)
{
	if(soundName == "Cherry")
		return 0;

	return -1;
}