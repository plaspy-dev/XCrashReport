#pragma once
#include <tchar.h>

// The archived Windows controls use the mutable return type of their CRT.
inline TCHAR* ArchivedTcsrchr(const TCHAR* text, TCHAR value)
{
    return const_cast<TCHAR*>(_tcsrchr(text, value));
}

inline TCHAR* ArchivedTcschr(const TCHAR* text, TCHAR value)
{
    return const_cast<TCHAR*>(_tcschr(text, value));
}

#undef _tcsrchr
#undef _tcschr
#define _tcsrchr ArchivedTcsrchr
#define _tcschr ArchivedTcschr
