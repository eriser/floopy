// OutputCache.h: interface for the COutputCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTCACHE_H__12D2CBC7_2DCE_4536_BC23_21C7FFD46676__INCLUDED_)
#define AFX_OUTPUTCACHE_H__12D2CBC7_2DCE_4536_BC23_21C7FFD46676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\IFloopy.h"

class COutputCache : public IFloopySoundFilter  
{
private:
	class CCachedBuffer
	{
	public:
		CCachedBuffer(char *signature, int position, BYTE *buff, int size);
		virtual ~CCachedBuffer();

		void GetBuffer(BYTE *buff, int size);
		void Add(char *signature, int position, BYTE *buff, int size);
		int Get(char *signature, int position, BYTE *buff, int size);

	private:
		char *m_pSignature;
		BYTE *m_buff;
		CCachedBuffer *m_Next[10];
	};

public:
	COutputCache();
	virtual ~COutputCache();

	void Add(char *signature, int position, BYTE *buff, int size);
	int Get(char *signature, int position, BYTE *buff, int size);

private:
	CCachedBuffer *m_pHead, *m_pTail;
};

#endif // !defined(AFX_OUTPUTCACHE_H__12D2CBC7_2DCE_4536_BC23_21C7FFD46676__INCLUDED_)