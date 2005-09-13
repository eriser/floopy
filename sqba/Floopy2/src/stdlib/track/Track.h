// Track.h: interface for the CTrack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACK_H__966CC29E_48E0_487B_B8B3_A7DDDF36967B__INCLUDED_)
#define AFX_TRACK_H__966CC29E_48E0_487B_B8B3_A7DDDF36967B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../ifloopy.h"

class CTrack : public IFloopySoundFilter  
{
public:
	CTrack();
	virtual ~CTrack();

	enumClassType GetType() { return TYPE_FLOOPY_SOUND_TRACK; }

	char *GetName()				{ return "stdlib.track"; }
	char *GetDescription()		{ return "Audio track"; }
	char *GetVersion()			{ return "0.1"; }
	char *GetAuthor()			{ return "sqba"; }

	bool ReadSourceIfDisabled()	{ return false; }

	int   GetPropertyCount();
	//bool  GetPropertyIndex(char *name, int *index);
	bool  GetPropertyVal(int index, float *value);
	void  SetPropertyVal(int index, float value);
	float GetPropertyMin(int index);
	float GetPropertyMax(int index);
	float GetPropertyStep(int index);
	char *GetPropertyName(int index);
	char *GetPropertyDesc(int index);
	//char *GetPropertyUnit(int index);

private:
	int m_nHeight;
	int m_nBGRed, m_nBGGreen, m_nBGBlue;
	int m_nForeRed, m_nForeGreen, m_nForeBlue;
};

#endif // !defined(AFX_TRACK_H__966CC29E_48E0_487B_B8B3_A7DDDF36967B__INCLUDED_)
