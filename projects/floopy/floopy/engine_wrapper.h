// Engine.h: interface for the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINE_H__2971634E_ED26_4A50_ABC1_123ADF851EA5__INCLUDED_)
#define AFX_ENGINE_H__2971634E_ED26_4A50_ABC1_123ADF851EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../engine/include/ifloopy.h"
#include "../engine/include/platform.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

class CEngineWrapper : public IFloopySoundEngine
{
public:
	CEngineWrapper(const char *plugin);
	virtual ~CEngineWrapper();

	const char *GetName()			{ return m_plugin->GetName(); }
	const char *GetDescription()	{ return m_plugin->GetDescription(); }
	const char *GetVersion()		{ return m_plugin->GetVersion(); }
	const char *GetAuthor()			{ return m_plugin->GetAuthor(); }

	IFloopySoundInput  *CreateInput(const char *plugin)
	{
		return (NULL != m_plugin ? m_plugin->CreateInput(plugin) : NULL);
	}

	IFloopySoundOutput *CreateOutput(const char *plugin, SOUNDFORMAT fmt)
	{
		return (NULL != m_plugin ? m_plugin->CreateOutput(plugin, fmt) : NULL);
	}

	IFloopySoundInput *GetSource() { return (m_plugin ? m_plugin->GetSource() : NULL); }

	bool Save(const char *filename) { return (m_plugin ? m_plugin->Save(filename) : false); }

	void Close() { if(m_plugin) m_plugin->Close(); }
	
private:
	void GetLibraryPath();

private:
	LIB_HANDLE			m_hModule;
	IFloopySoundEngine	*m_plugin;
	char				m_szPath[MAX_PATH];
};

#endif // !defined(AFX_ENGINE_H__2971634E_ED26_4A50_ABC1_123ADF851EA5__INCLUDED_)
