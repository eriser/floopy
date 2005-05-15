// Engine.h: interface for the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINE_H__2971634E_ED26_4A50_ABC1_123ADF851EA5__INCLUDED_)
#define AFX_ENGINE_H__2971634E_ED26_4A50_ABC1_123ADF851EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "../ifloopy.h"

class CEngine : public IFloopyEngine
{
public:
	CEngine();
	virtual ~CEngine();
/*
	char *GetName()			{ return "enginew"; }
	char *GetDescription()	{ return "Floopy Engine Plugin Wrapper"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }
*/
	char *GetName()			{ return m_plugin->GetName(); }
	char *GetDescription()	{ return m_plugin->GetDescription(); }
	char *GetVersion()		{ return m_plugin->GetVersion(); }
	char *GetAuthor()		{ return m_plugin->GetAuthor(); }

//	IFloopyEngine *getPlugin() { return m_plugin; }
	BOOL Create (char *plugin);

	IFloopySoundInput  *CreateInput(char *plugin)
	{
		return (NULL != m_plugin ? m_plugin->CreateInput(plugin) : NULL);
	}

	IFloopySoundOutput *CreateOutput(char *plugin, WAVFORMAT fmt)
	{
		return (NULL != m_plugin ? m_plugin->CreateOutput(plugin, fmt) : NULL);
	}

	IFloopySoundInput *GetSource() { return (m_plugin ? m_plugin->GetSource() : NULL); }

	BOOL Save(char *filename) { return (m_plugin ? m_plugin->Save(filename) : FALSE); }

	void Close() { if(m_plugin) m_plugin->Close(); }

private:
	HINSTANCE m_hinst;
	IFloopyEngine *m_plugin;
};

#endif // !defined(AFX_ENGINE_H__2971634E_ED26_4A50_ABC1_123ADF851EA5__INCLUDED_)
