// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include<Sddl.h>
#include<Ntsecapi.h>

#define LSA_LOOKUP_ISOLATED_AS_LOCAL 0x80000000
#define STATUS_SOME_NOT_MAPPED 0x00000107
#define STATUS_NONE_MAPPED 0xC0000073
