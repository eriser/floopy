#ifndef _FLOOPY_REVERSE_H_
#define _FLOOPY_REVERSE_H_

#include "../../ifloopy.h"

class CReverse : public IFloopySoundFilter  
{
public:
	CReverse();
	virtual ~CReverse();

	char *GetName()				{ return "stdlib.reverse"; }
	char *GetDescription()		{ return "Reverse"; }
	char *GetVersion()			{ return "0.1"; }
	char *GetAuthor()			{ return "sqba"; }

	bool ReadSourceIfDisabled()	{ return true; }

	int Read(BYTE*, int);
	void MoveTo(int);
	void Reset();
	int GetPosition();
	bool SetSource(IFloopySoundInput*);

private:
	void reverse(BYTE*, int);

private:
	int m_nPosition;
	BYTE *m_pBuffer;
	int m_nBuffSize;
	int m_nSamplesToBytes;
};

#endif // _FLOOPY_REVERSE_H_