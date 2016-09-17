#pragma once

#include <tchar.h>
#include <windows.h>


// Windows class
#define CUSTOM_WC	_T("CustomControl")


struct CustomData
{
	int id;
};

void CustomRegister(void);
void CustomUnregister(void);