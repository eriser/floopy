// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "Engine.h"
#include "Storage.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngine::CEngine()
{
	//m_offset = m_stopAt = m_length = 0;
	m_pFirst = m_pLast = NULL;
}

CEngine::~CEngine()
{
	if(m_source)
		delete m_source;

	tComponent *tmp;
	while(m_pFirst)
	{
		//printf("%d %d %f", first->offset, first->index, first->value);
		tmp = m_pFirst->next;
		delete m_pFirst;
		m_pFirst = tmp;
	}
}

tComponent *CEngine::add(IFloopy *comp)
{
	if(NULL == m_pLast)
	{
		m_pFirst = new tComponent;
		memset(m_pFirst, 0, sizeof(tComponent));
		m_pLast = m_pFirst;
		m_pFirst->comp = comp;
	}
	else
	{
		tComponent *tmp = new tComponent;
		memset(tmp, 0, sizeof(tComponent));
		tmp->prev = m_pLast;
		tmp->comp = comp;
		m_pLast->next = tmp;
		m_pLast = tmp;
	}
	return m_pLast;
}
/*
void CEngine::dump(FILE *fp)
{
	tComponent *tmp = m_pFirst;
	while(tmp)
	{
		tmp->comp->dump(fp);
		tmp = tmp->next;
	}
}
*/
IFloopySoundInput *CEngine::CreateInput(char *plugin)
{
	IFloopySoundInput *comp = new CInput(plugin);

	add(comp);

	return comp;
}

IFloopySoundOutput *CEngine::CreateOutput(char *plugin, WAVFORMAT fmt)
{
	IFloopySoundOutput *comp = new COutput(plugin, fmt);

	add(comp);

	return comp;
}

BOOL CEngine::Open(char *filename)
{
	if(0 == strcmpi(filename, "test"))
	{
		CStorage storage(this, "TEST");
		return storage.Load(filename);
	}

	// Check extension and then select apropriate plugin!
	CStorage storage(this, "xml_expat");
	return storage.Load(filename);
}

BOOL CEngine::Save(char *filename)
{
	if(0 == strcmpi(filename, "test"))
	{
		CStorage storage(this, "TEST");
		return storage.Save(filename);
	}

	// Check extension and then select apropriate plugin!
	CStorage storage(this, "xml_expat");
	return storage.Save(filename);
}

