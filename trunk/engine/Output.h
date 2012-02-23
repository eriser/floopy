// Output.h: interface for the COutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUT_H__8964D44D_9FFA_4F21_BA21_37E1D084CDD4__INCLUDED_)
#define AFX_OUTPUT_H__8964D44D_9FFA_4F21_BA21_37E1D084CDD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../ifloopy.h"
#include <stdio.h>
#include "timeline.h"
#include "PluginLoader.h"

/**
 * \class COutput
 * \brief Output plug-in wrapper class.
 * \author Filip Pavlovic
 * \version
 * \date 02. May 2005.
 *
 * Output plug-in wrapper class.
 */
class COutput : public IFloopySoundOutput, CPluginLoader
{
public:
	COutput();
	virtual ~COutput();

	bool Create(char *plugin, SOUNDFORMAT fmt);

	void Reset();

	bool Open(char *filename);
	//void Close();

	int Write(BYTE *data, int size);

	char *GetName();
	char *GetDescription();
	char *GetVersion();
	char *GetAuthor();

	int   GetParamCount();
	bool  GetParamVal(int index, float *value);
	void  SetParamVal(int index, float value);
	char *GetParamName(int index);
	char *GetParamDesc(int index);

	int   GetPropertyCount();
	bool  GetPropertyVal(int index, float *value);
	void  SetPropertyVal(int index, float value);
	char *GetPropertyName(int index);
	char *GetPropertyDesc(int index);

	void SetDest(IFloopySoundOutput *dst);

	char *GetLastErrorDesc();

	int GetPosition();

private:
	void copyErrorDesc();

private:
	char m_name[50];
	char m_szLastError[100];

	int m_samplesToBytes;
	int m_offset;
	IFloopySoundOutput *m_plugin;
	CTimeline m_timeline;
};

#endif // !defined(AFX_OUTPUT_H__8964D44D_9FFA_4F21_BA21_37E1D084CDD4__INCLUDED_)