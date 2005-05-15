// Mixer.cpp: implementation of the CInput class.
//
//////////////////////////////////////////////////////////////////////

#include "Input.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInput::CInput()
{
	m_nInputCount = 0;

	m_nBuffSize = 0;
	m_pBuffers = NULL;

	m_nLengths = NULL;
	m_nLengthsSize = 0;

#ifdef _DEBUG_TIMER_
	m_bDebugTimer = TRUE;
	m_nFrameSize=m_dwSpeed=m_nFrameCount=0;
#endif // _DEBUG_TIMER_
}

CInput::~CInput()
{
	for(int i=0; i<m_nInputCount; i++)
	{
		delete m_pInputs[i];
	}

	if(NULL != m_pBuffers)
		delete[] m_pBuffers;

	if(NULL != m_nLengths)
		delete[] m_nLengths;
}

int CInput::AddSource(IFloopySoundInput *src)
{
	if(m_nInputCount < MAX_INPUTS)
	{
		if(0==m_nInputCount)
			m_source = src;
		m_pInputs[m_nInputCount++] = src;
		return m_nInputCount;
	}
	return -1;
}

void CInput::RemoveSource(IFloopySoundInput *src)
{
	/*for(int i=0; i<m_nInputCount; i++)
	{
		if(src == m_pInputs[i])
		{
		}
	}*/
	/*if(index < m_nInputCount)
	{
		IFloopySoundInput *tmp = m_pInputs[index];
		tmp
		return m_pInputs[index];
	}*/
}

IFloopySoundInput *CInput::GetSource(int index)
{
	/*for(int i=0; i<m_nInputCount; i++)
	{
		if(index == i)
			return m_pInputs[i];
	}*/
	if(index < m_nInputCount)
		return m_pInputs[index];
	return NULL;
}

int CInput::GetInputCount()
{
	return m_nInputCount;
}

int CInput::Read(BYTE *data, int size)
{
	if(m_nLengthsSize != m_nInputCount)
	{
		if(NULL != m_nLengths)
			delete[] m_nLengths;
		m_nLengths = new int[m_nInputCount];
		m_nLengthsSize = m_nInputCount;
	}

	if(m_nInputCount>0)
	{
		WAVFORMAT *fmt = GetFormat();
		assert((fmt->bitsPerSample > 0) && (fmt->channels > 0));

		if(m_nBuffSize != (m_nInputCount*(int)size))
		{
			if(NULL != m_pBuffers)
				delete[] m_pBuffers;
			m_nBuffSize = m_nInputCount*size;
			m_pBuffers = new BYTE[m_nBuffSize];
		}
		assert(m_nBuffSize > 0);
		memset(m_pBuffers, 0, m_nBuffSize);
		BYTE *pm_pBuffers = m_pBuffers;

		// Fill source m_pBuffers;
		for(int i=0; i<m_nInputCount; i++)
		{
			m_nLengths[i] = m_pInputs[i]->Read(pm_pBuffers, size);
			pm_pBuffers += size;
		}

		mixBuffers(m_pBuffers, m_nInputCount, data, size);
	}

	int result = 0;
	for(int i=0; i<m_nInputCount; i++)
	{
		if(m_nLengths[i] > result)
			result = m_nLengths[i];
	}

	return result; 
}

void CInput::mixBuffers(BYTE *m_pBuffers, int buffCount, BYTE *output, int size)
{
#ifdef _DEBUG_TIMER_
	clock_t start = 0;
	if(m_bDebugTimer)
		start = clock();
#endif // _DEBUG_TIMER_

	WAVFORMAT *fmt = GetFormat();
	assert((fmt->bitsPerSample > 0) && (fmt->channels > 0));
	
	int step = fmt->bitsPerSample / 8;
	int numsamples = size/step;

	// For 16 bit samples only!!!
	short int *in  = (short int*)m_pBuffers;
	short int *out = (short int*)output;

	// For each sample
	for(int i=0; i<numsamples; i++)
	{
		// For each source
		for(int n=0; n<buffCount*numsamples; n+=numsamples)
		{
			*out += *(in+n+i) / buffCount;
		}
		out++; // Move to next sample
	}

#ifdef _DEBUG_TIMER_
	if(m_bDebugTimer)
	{
		m_dwSpeed += clock() - start;
		m_nFrameSize += numsamples;
		m_nFrameCount++;
	}
#endif // _DEBUG_TIMER_
}

/*
// Maybe this one is faster? Check out...
void CInput::mixBuffers(BYTE *m_pBuffers, int buffm_nInputCount, BYTE *output, int size)
{
	WAVFORMAT *fmt = GetFormat();
	assert((fmt->size > 0) && (fmt->channels > 0));
	
	int step = fmt->size / 8;
	int numsamples = size/step;

	// For 16 bit samples only!!!
	short int *in  = (short int*)m_pBuffers;
	short int *out = (short int*)output;

	// For each channel
	for(int i=0; i<buffm_nInputCount*numsamples; i+=numsamples)
	{
		short int *channel = in+i;
		out = (short int*)output;

		for(int n=0; n<numsamples; n++)
		{
			*(out++) += *(channel++) / buffm_nInputCount;
		}
	}
}
*/
void CInput::Close()
{
	for(int i=0; i<m_nInputCount; i++)
	{
		m_pInputs[i]->Close();
	}

#ifdef _DEBUG_TIMER_
	if(m_bDebugTimer)
	{
		float afmt = (float)m_dwSpeed / (float)m_nFrameCount;
		float afsz = (float)m_nFrameSize / (float)m_nFrameCount;
		float amr = afsz * (float)sampleSize() / (afmt / 1000.f);
		if(amr < 1024.f)
			printf("Mixing rate:\t%.2f b/sec\n", amr);
		else if(amr > 1024.f*1024.f)
			printf("Mixing rate:\t%.2f Mb/sec\n", amr / (1024.f*1024.f));
		else
			printf("Mixing rate:\t%.2f Kb/sec\n", amr / 1024.f);
		printf("Average frame mixing time:\t%f ms\n", afmt);
		printf("Average frame size:\t\t%.2f samples\n", afsz);
	}
	m_nFrameSize=m_dwSpeed=m_nFrameCount=0;
#endif // _DEBUG_TIMER_
}

void CInput::MoveTo(int samples)
{
	for(int i=0; i<m_nInputCount; i++)
	{
		m_pInputs[i]->MoveTo(samples);
	}
}

void CInput::Reset()
{
	for(int i=0; i<m_nInputCount; i++)
	{
		m_pInputs[i]->Reset();
	}
	if(m_nBuffSize > 0)
		memset(m_pBuffers, 0, m_nBuffSize);
}

int CInput::GetSize()
{
	int size = 0;
	for(int i=0; i<m_nInputCount; i++)
	{
		if(m_pInputs[i]->GetSize() > size)
			size = m_pInputs[i]->GetSize();
	}
	return size;
}

#ifdef _DEBUG_TIMER_
void CInput::SetParam(int index, float value)
{
	if(index == 0)
		m_bDebugTimer = (value==1.f);
}

float CInput::GetParam(int index)
{
	float afmt = (float)m_dwSpeed / (float)m_nFrameCount;
	float afsz = (float)m_nFrameSize / (float)m_nFrameCount;
	float amr = afsz * sampleSize() / (afmt / 1000.f);

	switch(index)
	{
	case -333:
		return (m_bDebugTimer ? 1.f : 0.f);
	case -334:
		return afmt;
	case -335:
		return afsz;
	case -336:
		return amr;
	}
	return 0.f;
}

char *CInput::GetParamName(int index)
{
	switch(index)
	{
	case -333:
		return "timer";
	case -334:
		return "mixtime";
	case -335:
		return "m_nFrameSize";
	case -336:
		return "mixrate";
	}
	return NULL;
}

char *CInput::GetParamDesc(int index)
{
	switch(index)
	{
	case -333:
		return "Debug Timer On/Off (1/0)";
	case -334:
		return "Average frame mixing time (in ms)";
	case -335:
		return "Average frame size (in bytes)";
	case -336:
		return "Average mixing rate (in bytes)";
	}
	return NULL;
}
#endif // _DEBUG_TIMER_
/*
WAVFORMAT *CInput::GetFormat()
{
	WAVFORMAT *fmt = IFloopySoundInput::GetFormat();
	//assert((fmt->bitsPerSample > 0) && (fmt->channels > 0));
	if(fmt->frequency > 0)
		return fmt;
	for(int i=0; i<m_nInputCount; i++)
	{
		WAVFORMAT *tmp = m_pInputs[i]->GetFormat();
		if(tmp->frequency > 0)
		{
			fmt = tmp;
			break;
		}
	}
	return fmt;
}
*/

int CInput::sampleSize()
{
	WAVFORMAT *fmt = GetFormat();
	assert(fmt->bitsPerSample > 0);
	return fmt->bitsPerSample / 8;
}
/*
int CInput::samplesToBytes()
{
	WAVFORMAT *fmt = GetFormat();
	assert((fmt->bitsPerSample > 0) && (fmt->channels > 0));
	return (fmt->bitsPerSample / 8) * fmt->channels;
}
*/