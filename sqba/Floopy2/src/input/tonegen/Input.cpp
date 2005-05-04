// Input.cpp: implementation of the CInput class.
//
//////////////////////////////////////////////////////////////////////

#include "Input.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInput::CInput()
{
	freq = 2600.0f;
	angle = 0.0;
}

CInput::~CInput()
{

}

UINT CInput::Read(BYTE *data, UINT size, UINT offset)
{
	//short sample_buffer[576*2];
	//short sample_buffer[8*2];
	short int *sample_buffer = (short*)data;

	if(-1 == offset)
		angle = 0.0;

	WAVFORMAT *fmt = GetFormat();
	UINT numsamples = size / ((fmt->size/8) * fmt->channels);


	int l = numsamples, x=l*fmt->channels;
	double dangle = M_PI * (double)fmt->channels * freq / (double)fmt->freq / 2.0;

	double max = (pow(2, fmt->size) - 1.0) / 2.0;

//	memset(data, 0, size);
	short int *s = sample_buffer;
	while (x--)
	{
		int i;
		double d = sin(angle)*max;
		//double d = sin(angle)*32766.5;
		__asm {
			fld d
			fistp i
		}
		*(s++) = i;
		angle += dangle;
	}

	return size;

//	fwrite(sample_buffer, 1, l, stdout);
}

int CInput::atoi(char *s)
{
	int n=0;
	while (s != NULL && *s >= '0' && *s <= '9')
	{
		n*=10;
		n += *s++ -'0';
	}
	return n;
}
