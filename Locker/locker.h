#pragma once
/**
	Author: Alwyn Dippenaar.
	Locks stuff.
**/

#ifndef _LOCKER_H
#define _LOCKER_H

#include <windows.h>
#include <tchar.h>
#include <process.h>

#ifndef SAFE_ZERO
#define SAFE_ZERO(obj, obj_type, obj_sz) ZeroMemory(obj, sizeof(obj_type) * obj_sz);
#endif

#endif