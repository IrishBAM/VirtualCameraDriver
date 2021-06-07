#include "Common.h"

DRIVERINTERFACE int Init(bool multiThread);
DRIVERINTERFACE int Free();
DRIVERINTERFACE int GetNumDevices();
DRIVERINTERFACE int GetDevicePath(int index, char* str, int maxLen);
DRIVERINTERFACE void DestroyDevice();
DRIVERINTERFACE int SetDevice(char* str, int strLen);
DRIVERINTERFACE int SetBuffer(void *data, unsigned long stride, unsigned long width, unsigned long height);
