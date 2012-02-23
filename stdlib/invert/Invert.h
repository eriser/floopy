// Invert.h: interface for the CInvert class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVERT_H__3B73BB16_5525_4827_A7B6_B4341803B836__INCLUDED_)
#define AFX_INVERT_H__3B73BB16_5525_4827_A7B6_B4341803B836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../ifloopy.h"

class CInvert : public IFloopySoundFilter  
{
public:
	CInvert();
	virtual ~CInvert();

	char *GetName()			{ return "stdlib.invert"; }
	char *GetDescription()	{ return "Invert"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }

	int Read(BYTE *data, int size);

	bool ReadSourceIfDisabled()	{ return true; }
};

#endif // !defined(AFX_INVERT_H__3B73BB16_5525_4827_A7B6_B4341803B836__INCLUDED_)
