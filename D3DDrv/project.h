#ifndef __project_h__
#define __project_h__

#ifdef _WIN32
   #pragma once
#else 

#include "dxstdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved )
{
    //@todo
    return TRUE;
}

#endif//__project_h__