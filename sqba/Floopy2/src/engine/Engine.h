// Engine.h: interface for the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINE_H__621A6F07_09D1_41D0_A981_DB32D29DA57A__INCLUDED_)
#define AFX_ENGINE_H__621A6F07_09D1_41D0_A981_DB32D29DA57A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "../ifloopy.h"
#include "input.h"
#include "output.h"
#include "timeline.h"

class CEngine : public IFloopyEngine
{
public:
	CEngine();
	virtual ~CEngine();

//	UINT Read(BYTE *data, UINT size, UINT offset);

	char *GetName()			{ return "enginewrapper"; }
	char *GetDescription()	{ return "Floopy Engine Plugin Wrapper"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }

//	int   GetParamCount()					{ return 1; }
//	void  SetParam(int index, float value)	{ if(index==0) m_startAt = value; if(index==2) m_stopAt = value; }
//	float GetParam(int index)				{ if(index==0) return m_startAt else if(index==2) return m_stopAt else return 0; }
//	char *GetParamName(int index)			{ return m_plugin->GetParamName(index); }
//	char *GetParamDesc(int index)			{ return m_plugin->GetParamDesc(index); }

	IFloopySoundInput  *CreateInput(char *plugin);
	IFloopySoundOutput *CreateOutput(char *plugin, WAVFORMAT *fmt);

	void MoveTo(UINT samples);
	void Reset();
	UINT Read(BYTE *data, UINT size);
	void SetSize(DWORD size);// { m_length = size; }
	DWORD GetSize() { return (m_length > 0 ? m_length : GetSource()->GetSize()); }

private:
//	void printPath(CInput *input, COutput *output);
//	void process(CInput *input, COutput *output);

	UINT m_offset;

	IFloopySoundInput *testCreateMaster();
	IFloopySoundInput *testCreateTrack1();
	IFloopySoundInput *testCreateTrack2();
	IFloopySoundInput *testCreateTrack3();
	IFloopySoundInput *testCreateTrack4(WAVFORMAT *fmt);

//	IFloopy *m_objects; // List

//	CInput *m_master;
	CTimeline m_timeline;

	UINT m_stopAt, m_length;
};

#endif // !defined(AFX_ENGINE_H__621A6F07_09D1_41D0_A981_DB32D29DA57A__INCLUDED_)
