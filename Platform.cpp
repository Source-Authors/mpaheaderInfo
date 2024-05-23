// Version 3, 29 June 2007
//
// Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
//
// Everyone is permitted to copy and distribute verbatim copies of this license
// document, but changing it is not allowed.
//
// This version of the GNU Lesser General Public License incorporates the terms
// and conditions of version 3 of the GNU General Public License, supplemented
// by the additional permissions listed below.

#include "stdafx.h"

#include "Platform.h"

#ifdef _WIN32
#include <Windows.h>
#endif

int GetLastSystemError() {
#ifdef _WIN32
  return ::GetLastError();
#else
  return errno;
#endif
}

void DumpSystemError(const char* error) {
#ifdef _WIN32
  ::MessageBox(NULL, error, _T("MPAFile Error"),
               MB_OK | MB_ICONERROR);
#else
  fprintf(stderr, "mp3: %s\n", error);
#endif
}