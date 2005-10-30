// ConvertToFloat.h: interface for the CConvertToFloat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERTTOFLOAT_H__01B73A29_0D03_4917_B461_1C361C6ACC79__INCLUDED_)
#define AFX_CONVERTTOFLOAT_H__01B73A29_0D03_4917_B461_1C361C6ACC79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../ifloopy.h"

#define SAMPLE_16BIT	short int
#define SAMPLE_8BIT		BYTE
#define FLOAT_32BIT		float //?

class CConvertToFloat : public IFloopySoundFilter
{
public:
	CConvertToFloat();
	virtual ~CConvertToFloat();

	char *GetName()			{ return "stdlib.tofloat"; }
	char *GetDescription()	{ return "Converts to float"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }

	int Read(BYTE *data, int size);

	bool ReadSourceIfDisabled()	{ return false; }

	SOUNDFORMAT *GetFormat();

private:
	int convert8bitToFloat(SAMPLE_8BIT *in, FLOAT_32BIT *out, int size);
	int convert16bitToFloat(SAMPLE_16BIT *in, FLOAT_32BIT *out, int size);

private:
	BYTE *m_pBuffer;
	int m_nBuffSize;
};

#endif // !defined(AFX_CONVERTTOFLOAT_H__01B73A29_0D03_4917_B461_1C361C6ACC79__INCLUDED_)
