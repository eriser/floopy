// WavFileOut.h: interface for the CWavFileOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVFILEOUT_H__73B41633_D564_4F1B_8602_A1A0CF7941FC__INCLUDED_)
#define AFX_WAVFILEOUT_H__73B41633_D564_4F1B_8602_A1A0CF7941FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* flags for wFormatTag field of WAVEFORMAT */
#define WAVE_FORMAT_PCM     1
#define MAX_PATH          260
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <windows.h>
//#include <mmsystem.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>

#include "../../ifloopy.h"
#include "headers.h"

class CWavFileOut : public IFloopySoundOutput
{
public:
	CWavFileOut(int nSamplesPerSec, int wBitsPerSample, int nChannels);
	virtual ~CWavFileOut();

	BOOL Open(char *filename);
	int Write(BYTE *data, int size);
	void Close();

	//char *GetName()			{ return "wavfile"; }
	char *GetName()			{ return (m_pFile ? m_filename : "wavfile"); }
	char *GetDescription()	{ return "WAV file writer"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }

private:
	FILE *m_pFile;
	RIFF m_riff;
	FMT  m_fmt;
	DATA m_data;
	char m_filename[MAX_PATH];
};

#endif // !defined(AFX_WAVFILEOUT_H__73B41633_D564_4F1B_8602_A1A0CF7941FC__INCLUDED_)
