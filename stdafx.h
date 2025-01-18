// GNU LESSER GENERAL PUBLIC LICENSE
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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

// dimhotepus: Custom fixes to compile without ATL.
#ifdef SA_MPA_BUILD_WITHOUT_ATL

#ifdef UNICODE

typedef wchar_t *PTSTR, *LPTSTR;
typedef const wchar_t *PCTSTR, *LPCTSTR;

#else /* UNICODE */

typedef char *PTSTR, *LPTSTR;
typedef const char *PCTSTR, *LPCTSTR;

#endif /* UNICODE */

// We are standard compliant.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

// dimhotepus: Add missed headers.
#ifdef _WIN32
#include <tchar.h>  // _T
#else
#define __T(x) x
#define _T(x) __T(x)
#define _TEXT(x) __T(x)
#endif

#include <cassert>  // assert
#include <cerrno>   // errno
#include <cstring>  // memcmp
#include <cstdio>   // sprintf_s
#include <cstdlib>  // atoi, malloc
#include <string>

#include <system_error>

#define CString std::string
#define MPA_USE_STRING_FOR_CSTRING 1

#else  // Original code.

#pragma once

// Prevent tons of unused windows definitions
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Strict types mode.
#ifndef STRICT
#define STRICT
#endif

// Access to Windows 10 features.
#define _WIN32_WINNT 0x0A00

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN  // Exclude rarely-used stuff from Windows headers
#endif

// Some CString constructors will be explicit
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

// Turns off MFC's hiding of some common and often safely ignored warning
// messages.
#define _AFX_ALL_WARNINGS

#include <cassert>       // assert
#include <stdexcept>     // std::length_error, etc.
#include <system_error>  // std::system_category, etc.

#include <afxwin.h>   // MFC core and standard components
#include <afxext.h>   // MFC extensions
#include <afxdisp.h>  // MFC Automation classes

#include <afxdtctl.h>  // MFC support for Internet Explorer 4 Common Controls

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>  // MFC support for Windows Common Controls
#endif               // _AFX_NO_AFXCMN_SUPPORT

#endif  // !SA_MPA_BUILD_WITHOUT_ATL
