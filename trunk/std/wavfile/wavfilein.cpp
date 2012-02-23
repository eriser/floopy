// WavFileIn.cpp: implementation of the CWavFileIn class.
//
//////////////////////////////////////////////////////////////////////

#include <string.h>
#include <assert.h>

#include "wavfilein.h"


struct chunk_hdr	// CHUNK 8-byte header
{
	char	id[4];	// identifier, e.g. "fmt" or "data"
	DWORD	len;	// remaining chunk length after header
};

struct WAVEFORM
{
	WORD  wFormatTag;		//format of digital sound
	WORD  nChannels;		//Number of channels (1 for mono and 2 for stereo)
	DWORD nSamplesPerSec;	//Number of samples per second
	DWORD nAvgBytesPerSec;	//Average number bytes of data per second
	WORD  nBlockAlign;		//Minimal data size for playing
	WORD  wBitsPerSample;	//Bits per sample (8 or 16)
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWavFileIn::CWavFileIn()
{
	m_pFile				= NULL;
	m_size				= 0;
	m_nSamplesToBytes	= 0;
	m_nHeaderLength		= 0;
	m_iDataSize			= 0;
	memset(m_filename, 0, sizeof(m_filename));
}

CWavFileIn::~CWavFileIn()
{
	if( m_pFile )
	{
		fclose(m_pFile);
	}
}

bool CWavFileIn::Open(const char *filename)
{
	m_pFile = fopen(filename, "rb");
	if(NULL == m_pFile)
	{
		fprintf(stderr, "File '%s' not found.", filename);
		return false;
	}

	chunk_hdr hdr;

	while( !feof(m_pFile) )
	{
		memset(&hdr, 0, sizeof(chunk_hdr));

		if(fread( &hdr, sizeof(chunk_hdr), 1, m_pFile ) <= 0)
			break;

		if(0 == strncmp("RIFF", hdr.id, 4))
		{
			char wave_id[4] = {0};
			int numread = fread( wave_id, sizeof(char), 4, m_pFile );

			if(numread != 4)
				return false;

			if(0 != strncmp("WAVE", wave_id, 4))
				return false;
		}
		else if(0 == strncmp("fmt", hdr.id, 3))
		{
			WAVEFORM format;
			memset(&format, 0, sizeof(WAVEFORM));
			int numread = fread( &format, sizeof(WAVEFORM), 1, m_pFile );

			if(numread != 1)
				return false;

			m_format.format			= format.wFormatTag;
			m_format.channels		= (int)format.nChannels;
			m_format.frequency		= format.nSamplesPerSec;
			m_format.bitsPerSample	= format.wBitsPerSample;

			if( !m_format.format || !m_format.channels ||
				!m_format.frequency || !m_format.bitsPerSample )
				return false;

			m_nSamplesToBytes = ((m_format.bitsPerSample/8) * m_format.channels);

			// Skip the rest of the structure
			int rest = hdr.len - sizeof(WAVEFORM);
			if(rest > 0)
				fseek(m_pFile, rest, SEEK_CUR);
		}
		else if(0 == strncmp("data", hdr.id, 4))
		{
			if(0 == m_nSamplesToBytes)
				return false;
			m_nHeaderLength = ftell(m_pFile);
			m_iDataSize = hdr.len;
			m_size = hdr.len / m_nSamplesToBytes; // Number of samples
			break;
		}
		else
		{
			// Unknown header, skip it.
			fseek(m_pFile, hdr.len, SEEK_CUR);
		}
	}

	if(0==m_nHeaderLength || m_iDataSize==0)
		return false;

	memset(m_filename, 0, sizeof(m_filename));
	strcpy(m_filename, filename);

	return true;
}

void CWavFileIn::Reset()
{
//	static int count = 0;
//	printf("CWavFileIn::Reset(%d)\n", count++);
//	assert(count < 28);
	assert(m_pFile);
	int res = fseek(m_pFile, m_nHeaderLength, SEEK_SET);
	assert(0 == res);
}

int CWavFileIn::Read(BYTE *data, int size)
{
	if( !m_pFile )
		return 0;

	long pos = ftell(m_pFile);
	long len = m_iDataSize + m_nHeaderLength;
	if(pos+(long)size > len)
	{
		size = len - pos;
	}

	return fread( data, 1, size, m_pFile );
}

int CWavFileIn::GetSize()
{
	return m_size; // In samples!
}

void CWavFileIn::MoveTo(int samples)
{
	assert(m_pFile);
	int n = samples * m_nSamplesToBytes;
	int res = fseek(m_pFile, m_nHeaderLength+n, SEEK_SET);
	assert(0 == res);
}

int CWavFileIn::GetPosition()
{
	if( !m_pFile )
		return EOF;
	long pos = ftell(m_pFile);
	return (pos - m_nHeaderLength) / m_nSamplesToBytes;
}

void CWavFileIn::Close()
{
	if( m_pFile )
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

bool CWavFileIn::IsEOF()
{
	return (m_pFile ? 0!=feof(m_pFile) : true);
}

int CWavFileIn::split_channels(BYTE *input, int len, BYTE **output, int channels, int samples)
{
	SOUNDFORMAT *fmt = GetFormat();
	if(!fmt || !fmt->bitsPerSample || !fmt->channels)
		return 0;

	switch(fmt->bitsPerSample)
	{
		case 8:
			{
				split_channels2(input, len, output, channels, samples);
			}
			break;
		case 16:
			{
				split_channels2(input, len, (WORD**)output, channels, samples);
			}
			break;
	}

	return 0;
}

template<typename T>
int CWavFileIn::split_channels2(BYTE *input, int len, T **output, int channels, int samples)
{
	SOUNDFORMAT *fmt = GetFormat();
	if(!fmt || !fmt->channels)
		return 0;

	T *sample = (T*)input;
	T *end_sample = sample + len/sizeof(T) - 1;
	int index = 0;
	do {
		int channel = 0;
		do {
			output[channel++][index] = *(sample++);
		} while(channel<fmt->channels && channel<channels);
		index++;
	} while(sample!=end_sample && index<samples);

	return 0;
}
