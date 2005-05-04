// Input.h: interface for the CInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUT_H__579B912B_2195_41EE_95BE_0B229EB45826__INCLUDED_)
#define AFX_INPUT_H__579B912B_2195_41EE_95BE_0B229EB45826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>

#include "../../ifloopy.h"
#include "headers.h"

class CInput : public IFloopySoundInput
{
public:
	CInput();
	virtual ~CInput();

	BOOL Open(char *filename);
	DWORD GetSize();
	UINT Read(BYTE *data, UINT size, UINT offset);
	void Close();

	void Reset();

	char *GetName()			{ return "wavfile"; }
	char *GetDescription()	{ return "WAV file reader"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }

private:
	UINT m_size;	// Number of samples
	FILE *m_pFile;
	RIFF m_riff;
	FMT  m_fmt;
	DATA m_data;
};

#endif // !defined(AFX_INPUT_H__579B912B_2195_41EE_95BE_0B229EB45826__INCLUDED_)
