#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>

#ifndef INDLL_H
#define INDLL_H

#define DllExport   __declspec( dllexport )

class DllExport testClass {

public:
	testClass() {}
	int i;
	virtual void func(void) { std::cout << "Dll class" << std::endl; }
};

/*
#ifdef EXPORTING_DLL
extern __declspec(dllexport) void HelloWorld();
#else
extern __declspec(dllimport) void HelloWorld();
#endif
*/
#endif