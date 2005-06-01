// Engine.h: interface for the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINE_H__621A6F07_09D1_41D0_A981_DB32D29DA57A__INCLUDED_)
#define AFX_ENGINE_H__621A6F07_09D1_41D0_A981_DB32D29DA57A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <time.h>
#include "../ifloopy.h"
#include "input.h"
#include "output.h"
#include "timeline.h"

#define NAME_LEN	50

enum enumObjType { TYPE_INPUT, TYPE_OUTPUT, TYPE_ENGINE };

/**
 * Structure used for keeping information
 * about all objects created by this engine
 */
struct tComponent
{
	IFloopy		*obj;	/** Pointer to the object itself	*/
	tComponent	*prev;	/** Pointer to the next object		*/
	tComponent	*next;	/** Pointer to the previous object	*/
	enumObjType	type;	/** Object type						*/
};

class CEngine : public IFloopySoundEngine
{
public:
	CEngine(HMODULE hModule);
	virtual ~CEngine();

	char *GetName()			{ return "engine"; }
	char *GetDescription()	{ return "Floopy Engine"; }
	char *GetVersion()		{ return "0.1"; }
	char *GetAuthor()		{ return "sqba"; }

	BOOL Open(char *filename);
	BOOL Save(char *filename);
	void Close();

	IFloopySoundInput  *CreateInput(char *plugin);
	IFloopySoundOutput *CreateOutput(char *plugin, SOUNDFORMAT fmt);

	void SetParamAt(IFloopy *obj, int offset, int index, float value);
	void ResetParamAt(IFloopy *obj, int offset, int index);
	void EnableAt(IFloopy *obj, int offset, BOOL bEnable);

	char *GetDisplayName();
	void SetDisplayName(char *name, int len);

	void RegisterUpdateCallback(UpdateCallback func);

	char *GetLastErrorDesc();
	//int GetLastError();
	//BOOL GetLastError(char *str, int len);

	BOOL ReadSourceIfDisabled()	{ return FALSE; }

//	int GetSize();

private:
//	int samplesToBytes();
	char *getPluginName(char *filename);
//	enumObjType createObject(char *filename);
	tComponent *add(IFloopy *comp, enumObjType type);
	void saveChildEngines();
	//void setLastError(char *err, char *str);

private:
	HMODULE m_hModule;				/** Handle to the module that created the engine */
	char m_szDisplayname[NAME_LEN];	/** Engine name, user defined string */
	char m_szLastError[100];		/** Last error description */
	char m_szPath[MAX_PATH];		/** Physical location of the engine */
	char m_szFileName[MAX_PATH];	/** File name, set after succesfull call to Open() */
	CTimeline m_timeline;			/** Parameter changes */
	UpdateCallback m_callback;		/** Callback function called on parameter changes while playing */
	tComponent *m_pFirst, *m_pLast;	/** Linked list of all objects created by the engine */
};

#endif // !defined(AFX_ENGINE_H__621A6F07_09D1_41D0_A981_DB32D29DA57A__INCLUDED_)
