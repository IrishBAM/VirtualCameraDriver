#include "Common.h"
#include "DeviceEnumeration.h"
#include "Device.h"

#define NUM_MAX_PATHS 16
static std::string cachedPaths[NUM_MAX_PATHS];
static int numDevices;

static Device* activeDevice = NULL;

#define TEMPORARY_BUFFER_SIZE (VIRTUALCAMERA_WIDTH * VIRTUALCAMERA_HEIGHT * 3)
static PVOID temporaryBuffer = NULL;

DRIVERINTERFACE int Init(bool multiThread)
{
	HRESULT hr = multiThread ? CoInitializeEx(NULL, COINIT_MULTITHREADED) :
		CoInitialize(NULL);
	if (!SUCCEEDED(hr))
	{
		return 0;
	}

	numDevices = EnumerateDevicePaths(cachedPaths, NUM_MAX_PATHS);
	if (numDevices < 0) {
		return 0;
	}

	temporaryBuffer = malloc(TEMPORARY_BUFFER_SIZE);
	if (temporaryBuffer == NULL)
	{
		return 0;
	}

	return 1;
}

DRIVERINTERFACE int Free()
{
	if (activeDevice != NULL) 
	{
		delete activeDevice;
	}

	free(temporaryBuffer);

	CoUninitialize(); 

	return 1;
}

DRIVERINTERFACE int GetNumDevices()
{
	return numDevices;
}

DRIVERINTERFACE int GetDevicePath(int index, char* str, int maxLen)
{
	if (index < 0 || index >= numDevices) {
		return -1;
	}

	strcpy_s(str, maxLen, cachedPaths[index].c_str());

	return 1;
}

DRIVERINTERFACE void DestroyDevice()
{
	if (activeDevice != NULL)
	{
		delete activeDevice;
		activeDevice = NULL;
	}
}

DRIVERINTERFACE int SetDevice(char* str, int strLen) 
{
	DestroyDevice();

	IBaseFilter* filter = NULL;
	if (!GetFilter(std::string(str), &filter) || filter == NULL)
	{
		return 0;
	}

	activeDevice = new Device(filter);
	if (!activeDevice->Init()) 
	{
		delete activeDevice;
		activeDevice = NULL;

		return 0;
	}

	return 1;
}

DRIVERINTERFACE int SetBuffer(void *data, unsigned long stride, unsigned long width, unsigned long height)
{
	if (activeDevice == NULL) 
	{
		return -1;
	}

	if (width != VIRTUALCAMERA_WIDTH || height != VIRTUALCAMERA_HEIGHT)
	{
		return -1;
	}

	memset(temporaryBuffer, 0x00, TEMPORARY_BUFFER_SIZE);

	PUCHAR inputData = (PUCHAR)data;
	PUCHAR buffer = (PUCHAR)temporaryBuffer;
	for (ULONG y = 0; y < height; y++)
	{
		PUCHAR sourceLine = inputData + stride * y;
		PUCHAR targetLine = buffer + ((VIRTUALCAMERA_WIDTH * 3) * y);
		memcpy(targetLine, sourceLine, VIRTUALCAMERA_WIDTH * 3);
	}

	activeDevice->SetData(temporaryBuffer, TEMPORARY_BUFFER_SIZE);

	return 1;
}