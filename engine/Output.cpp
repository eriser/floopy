// Output.cpp: implementation of the COutput class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <time.h>
#include "Output.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef IFloopySoundOutput* (*CreateProc)(char*, SOUNDFORMAT);
#define PROC_NAME "CreateOutput"
#define PLUG_EXT ".dll"

COutput::COutput()
{
	m_plugin			= NULL;
	m_offset			= 0;
	m_samplesToBytes	= 0;
}

COutput::~COutput()
{
	if(NULL != m_plugin)
		delete m_plugin;
}


bool COutput::Create(char *plugin, SOUNDFORMAT fmt)
{
	if(NULL == plugin)
	{
		sprintf(m_szLastError, "plugin is NULL.\n\0");
		return false;
	}

	if(fmt.bitsPerSample == 0)
	{
		sprintf(m_szLastError, "fmt.bitsPerSample == 0.\n\0");
		return false;
	}

	if(fmt.channels == 0)
	{
		sprintf(m_szLastError, "fmt.channels == 0.\n\0");
		return false;
	}

	if(fmt.format == 0)
	{
		sprintf(m_szLastError, "fmt.format == 0.\n\0");
		return false;
	}

	if(fmt.frequency == 0)
	{
		sprintf(m_szLastError, "fmt.frequency == 0.\n\0");
		return false;
	}

	char *library = plugin;

	char *sep = strrchr(plugin, '.');
	if(sep)
	{
		char tmp[MAX_PATH] = {0};
		strcpy(tmp, plugin);
		char *sep = strrchr(tmp, '.');
		plugin = sep+1;
		*sep = 0;
		library = tmp;
	}
	char *filename = new char[strlen(library) + 5];
	strcpy(filename, library);
	strcat(filename, PLUG_EXT);
	
	if( !LoadPlugin(filename) )
	{
		sprintf(m_szLastError, "File not found: %s.\n\0", filename);
		delete filename;
		return false;
	}
	delete filename;

	CreateProc func = (CreateProc)GetFunction(PROC_NAME);
	if(func == NULL)
	{
		sprintf(m_szLastError, "Function %s() not found in file: %s.\n\0", PROC_NAME, filename);
		return false;
	}

	m_plugin = func( plugin, fmt );
	IFloopySoundOutput::SetDest( m_plugin );
	m_samplesToBytes = (fmt.bitsPerSample / 8) * fmt.channels;
	return true;
}

int COutput::Write(BYTE *data, int size)
{
	if(NULL!=m_plugin && m_samplesToBytes>0)
	{
		int len = m_plugin->Write(data, size);
		if(len != size)
			copyErrorDesc();
		m_offset += len / m_samplesToBytes;
		return len;
	}
	return 0;
}

void COutput::Reset()
{
	m_offset = 0;
	m_plugin->Reset();
}

bool COutput::Open(char *filename)
{
	bool bResult = false;

	if(NULL == m_plugin)
		sprintf(m_szLastError, "NULL == m_plugin.\n\0");
	else
	{
		bResult = m_plugin->Open( filename );
		if( !bResult )
			copyErrorDesc();
	}

	return bResult;
}

void COutput::copyErrorDesc()
{
	strnset(m_szLastError, 0, 100);

	if(NULL == m_plugin)
		sprintf(m_szLastError, "Plugin not created.\n\0");
	else
	{
		char *err = m_plugin->GetLastErrorDesc();
		if(NULL != err)
			strncpy(m_szLastError, err, 99);
	}
}

char *COutput::GetName()
{
	char *result = "Plugin not created.\n\0";
	if(NULL != m_plugin)
		result = m_plugin->GetName();
	else
		copyErrorDesc();
	return result;
}

char *COutput::GetDescription()
{
	char *result = "Plugin not created.\n\0";
	if(NULL != m_plugin)
		result = m_plugin->GetDescription();
	else
		copyErrorDesc();
	return result;
}

char *COutput::GetVersion()
{
	char *result = "Plugin not created.\n\0";
	if(NULL != m_plugin)
		result = m_plugin->GetVersion();
	else
		copyErrorDesc();
	return result;
}

char *COutput::GetAuthor()
{
	char *result = "Plugin not created.\n\0";
	if(NULL != m_plugin)
		result = m_plugin->GetAuthor();
	else
		copyErrorDesc();
	return result;
}

int COutput::GetParamCount()
{
	return NULL!=m_plugin ? m_plugin->GetParamCount() : 0;
}

bool COutput::GetParamVal(int index, float *value)
{
	return NULL!=m_plugin ? m_plugin->GetParamVal(index, value) : false;
}

void COutput::SetParamVal(int index, float value)
{
	if(NULL != m_plugin)
		m_plugin->SetParamVal(index, value);
}

char *COutput::GetParamName(int index)
{
	return NULL!=m_plugin ? m_plugin->GetParamName(index) : NULL;
}

char *COutput::GetParamDesc(int index)
{
	return NULL!=m_plugin ? m_plugin->GetParamDesc(index) : NULL;
}

int COutput::GetPropertyCount()
{
	return NULL!=m_plugin ? m_plugin->GetPropertyCount() : 0;
}

bool COutput::GetPropertyVal(int index, float *value)
{
	return NULL!=m_plugin ? m_plugin->GetPropertyVal(index, value) : false;
}

void COutput::SetPropertyVal(int index, float value)
{
	if(NULL != m_plugin)
		m_plugin->SetPropertyVal(index, value);
}

char *COutput::GetPropertyName(int index)
{
	return NULL!=m_plugin ? m_plugin->GetPropertyName(index) : NULL;
}

char *COutput::GetPropertyDesc(int index)
{
	return NULL!=m_plugin ? m_plugin->GetPropertyDesc(index) : NULL;
}

void COutput::SetDest(IFloopySoundOutput *dst)
{
	if(NULL != m_plugin)
		m_plugin->SetDest(dst);
}

char *COutput::GetLastErrorDesc()
{
	return m_szLastError;
}

int COutput::GetPosition()
{
	return NULL!=m_plugin ? m_plugin->GetPosition() : 0;
}
