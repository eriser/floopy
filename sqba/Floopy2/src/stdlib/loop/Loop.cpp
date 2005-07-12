// Loop.cpp: implementation of the CLoop class.
//
//////////////////////////////////////////////////////////////////////

#include "loop.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoop::CLoop()
{
	m_nPosition = 0;
//	m_nLoops = 0;
//	m_nMaxLoops = -1;  // -1 - infinite?
}

CLoop::~CLoop()
{

}

int CLoop::Read(BYTE *data, int size)
{
//	if((m_nMaxLoops > 0) && (m_nLoops >= m_nMaxLoops))
//		return 0;

	int len = 0;
/*	int origSize = size;
	
	int srclen = IFloopySoundFilter::GetSize();
	srclen *= samplesToBytes();

	if((m_nPosition + size) > srclen)
	{
		if(srclen > m_nPosition)
		{
			size = srclen - m_nPosition;
			len = IFloopySoundFilter::Read(data, size);
			size = origSize - size;
		}
		Reset();
//		m_nPosition = 0;
//		m_nLoops++;
		len += IFloopySoundFilter::Read(data, size);
		m_nPosition += len;
	}
	else
	{*/
		//len = IFloopySoundFilter::Read(data, size);

		//if(EOF == len)
		//	return len;

		int result = 0;
		while(result < size)
		{
			len = IFloopySoundFilter::Read(data+result, size-result);

			if(EOF == len)
			{
				Reset();
				len = 0;
			}
			else
			{
				m_nPosition += len;
				result += len;
			}
		}
//	}

	return result;
}
/*
void CLoop::SetParam(int index, float value)
{ 
	m_nMaxLoops = (int)value;
	Reset();
}
*/
int CLoop::GetSize()
{
//	if(m_nMaxLoops > 0)
//		return (NULL != m_source ? m_source->GetSize()*m_nMaxLoops : 0);
//	else
		return SIZE_INFINITE;
}

void CLoop::MoveTo(int samples)
{
	int size = IFloopySoundFilter::GetSize();

	/*if((m_nMaxLoops > 0) && (samples > (size * m_nMaxLoops)))
	{
		Reset();
		m_nLoops = m_nMaxLoops;
		return;
	}*/

	if(samples > size)
	{
		//m_nLoops = samples / size;
		/*int n = samples % size;
		float a = (float)samples / (float)size;
		int b = (int)samples / (int)size;
		float c = a - (float)b;
		//samples -= (m_nLoops * size);
		int x = samples - (samples % size);
		samples = size * (samples % size) / 10;*/
		//samples = size - (samples % size);
		samples = samples % size;
		IFloopySoundFilter::Reset();
	}
	IFloopySoundFilter::MoveTo(samples);

//	m_nPosition = samples * samplesToBytes();
}

/*
void CLoop::MoveTo(int samples)
{
	int size = IFloopySoundFilter::GetSize();

	if((samples > (size * m_nMaxLoops)) && (m_nMaxLoops > 0))
	{
		Reset();
		m_nLoops = m_nMaxLoops;
		return;
	}

	if(samples > size)
	{
		int s = samples / size;
		m_nLoops = s;
		if(s>0)
			samples -= (s * size);
		Reset();
	}
	IFloopySoundFilter::MoveTo(samples);

	m_nPosition = samples * samplesToBytes();
}
*/
void CLoop::Reset()
{
	m_nPosition = 0;
//	m_nLoops=0;
	IFloopySoundFilter::Reset();
}

int CLoop::samplesToBytes()
{
	SOUNDFORMAT *fmt = GetFormat();
	assert((fmt->bitsPerSample > 0) && (fmt->channels > 0));
	return (fmt->bitsPerSample / 8) * fmt->channels;
}
