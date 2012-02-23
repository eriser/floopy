// Input.cpp: implementation of the CInput class.
//
//////////////////////////////////////////////////////////////////////

#include "Input.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInput::CInput()
{
	m_volume = 100;
}

CInput::~CInput()
{

}

int CInput::Read(BYTE *data, int size)
{
	int len = IFloopySoundInput::Read(data, size);
	if(EOF == len)
		return len;

	short int *sample = (short int*)data;

	SOUNDFORMAT *fmt = GetFormat();
	//assert((fmt->bitsPerSample > 0) && (fmt->channels > 0));
	if((fmt->bitsPerSample == 0) || (fmt->channels == 0))
		return 0;
	//int numsamples = size / ((fmt->size/8) * fmt->channels);
	int numsamples = len / (fmt->bitsPerSample/8);
	//int numsamples = len / ((fmt->size / 8) * GetFormat()->channels);

//	int max = (int)pow(2, fmt->size) / 2;

	float percent = (float)m_volume / 100.f;
//	float coef = (float)max * (float)m_volume / 100.f;

	int i = numsamples;
	while(i--)
	{
		//*(sample++) += ((float)*sample * coef);
		//*(sample++) += coef;
		//*(sample++) += ((float)*sample * (float)m_volume / 100.f);
		//if(*sample < 100 || *sample > -100)
		//	*sample = *sample;
//		*(sample++) = ((float)*sample * percent);

		/*if(m_volume == 120.00)
		{
			short int d = ((float)*sample * percent);
		}*/

		*sample = ((float)*sample * percent);
		sample++;

		/*short int s = (short int)((float)*sample * m_volume);
		if((s > -max) && (s < max))
			*sample = s;
		sample++;*/
	}
	return len;
}