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
  ::OutputDebugString(error);

  ::MessageBox(NULL, error, _T("Mpeg Audio Info Error"),
               MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
  fprintf(stderr, "mp3: %s\n", error);
#endif
}