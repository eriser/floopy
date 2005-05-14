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

struct tComponent
{
	IFloopy *comp;
	tComponent *prev;
	tComponent *next;
	BOOL bInput;
};

class CEngine : public IFloopyEngine
{
public:
	CEngine();
	virtual ~CEngine();

	char *GetName()			{ return "engine"; }
	char *GetDescription()	{ return "Floopy Engine"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }

	BOOL Open(char *filename);
	BOOL Save(char *filename);

//	int   GetParamCount()					{ return 1; }
//	void  SetParam(int index, float value)	{ if(index==0) m_startAt = value; if(index==2) m_stopAt = value; }
//	float GetParam(int index)				{ if(index==0) return m_startAt else if(index==2) return m_stopAt else return 0; }
//	char *GetParamName(int index)			{ return m_plugin->GetParamName(index); }
//	char *GetParamDesc(int index)			{ return m_plugin->GetParamDesc(index); }

	IFloopySoundInput  *CreateInput(char *plugin);
	IFloopySoundOutput *CreateOutput(char *plugin, WAVFORMAT fmt);

	void SetParamAt(IFloopy *obj, int offset, int index, float value);
	void EnableAt(IFloopy *obj, int offset, BOOL bEnable);

	void Close();

private:
	char *getStorageName(char *filename);

private:
	CTimeline m_timeline;

	IFloopySoundInput *testCreateMaster();
	IFloopySoundInput *testCreateTrack1();
	IFloopySoundInput *testCreateTrack2();
	IFloopySoundInput *testCreateTrack3();
	IFloopySoundInput *testCreateTrack4(WAVFORMAT *fmt);

	tComponent *m_pFirst, *m_pLast;
	tComponent *add(IFloopy *comp, BOOL bInput);
};

#endif // !defined(AFX_ENGINE_H__621A6F07_09D1_41D0_A981_DB32D29DA57A__INCLUDED_)
