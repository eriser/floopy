// Convert8to16bit.h: interface for the CConvert8to16bit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERT8TO16BIT_H__44F38EC4_95C1_46F4_9AAD_5C488BC88E0F__INCLUDED_)
#define AFX_CONVERT8TO16BIT_H__44F38EC4_95C1_46F4_9AAD_5C488BC88E0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <assert.h>
#include "../../ifloopy.h"

class CConvert8to16bit : public IFloopySoundFilter  
{
public:
	CConvert8to16bit();
	virtual ~CConvert8to16bit();

	char *GetName()			{ return "stdlib.8to16bit"; }
	char *GetDescription()	{ return "Converts 8bit samples to 16bit"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }

	int Read(BYTE *data, int size);

	BOOL ReadSourceIfDisabled()	{ return FALSE; }

	SOUNDFORMAT *GetFormat();

//	BOOL SetSource(IFloopySoundInput *src);

private:
	BYTE *m_pBuffer;
	int m_nBuffSize;
};

#endif // !defined(AFX_CONVERT8TO16BIT_H__44F38EC4_95C1_46F4_9AAD_5C488BC88E0F__INCLUDED_)
