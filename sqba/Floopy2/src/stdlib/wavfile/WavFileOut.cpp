// WavFileOut.cpp: implementation of the CWavFileOut class.
//
//////////////////////////////////////////////////////////////////////

#include "wavfileout.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWavFileOut::CWavFileOut(int nSamplesPerSec, int wBitsPerSample, int nChannels)
{
	m_pFile = NULL;

	memset(&m_riff, 0, sizeof(RIFF));
	memset(&m_fmt, 0, sizeof(FMT));
	memset(&m_fmt.fmtFORMAT, 0, sizeof(WAVEFORM));
	memset(&m_data, 0, sizeof(DATA));

	strcpy(m_riff.riffID, "RIFF");
	//riff.riffSIZE = 0;//File size minus 8 bytes
	strcpy(m_riff.riffFORMAT, "WAVE");

	// set up the WAVEFORM structure.
	strcpy(m_fmt.fmtID, "fmt ");
	m_fmt.fmtSIZE = 16;
	m_fmt.fmtFORMAT.nSamplesPerSec  = 44100;	// sample rate
	m_fmt.fmtFORMAT.wBitsPerSample  = 16;		// sample size
	m_fmt.fmtFORMAT.nChannels       = 2;		// channels
	m_fmt.fmtFORMAT.wFormatTag      = WAVE_FORMAT_PCM;
	m_fmt.fmtFORMAT.nBlockAlign     = (m_fmt.fmtFORMAT.wBitsPerSample * m_fmt.fmtFORMAT.nChannels) >> 3;
	m_fmt.fmtFORMAT.nAvgBytesPerSec = m_fmt.fmtFORMAT.nBlockAlign * m_fmt.fmtFORMAT.nSamplesPerSec;

	strcpy(m_data.dataID, "data");
	m_data.dataSIZE = 0;

	memset(m_filename, 0, sizeof(m_filename));
}

CWavFileOut::~CWavFileOut()
{

}

bool CWavFileOut::Open(char *filename)
{
	m_pFile = fopen(filename, "wb");
	if(NULL != m_pFile)
	{
		int i = fseek(m_pFile, 0, SEEK_SET);

		fwrite(&m_riff, sizeof(RIFF), 1, m_pFile);
		fwrite(&m_fmt, sizeof(FMT), 1, m_pFile);
		fwrite(&m_data, sizeof(DATA), 1, m_pFile);

		memset(m_filename, 0, sizeof(m_filename));
		strncpy(m_filename, filename, MAX_PATH);

		return true;
	}
	return false;
}

int CWavFileOut::Write(BYTE *data, int size)
{
	int written = 0;
	if(NULL != m_pFile)
	{
		written = fwrite(data, 1, size, m_pFile);
		m_data.dataSIZE += written;//(written * size);
		fflush(m_pFile);
	}
	return written;
}

void CWavFileOut::Close()
{
	if(NULL != m_pFile)
	{
		m_riff.riffSIZE = m_data.dataSIZE + sizeof(RIFF) + sizeof(FMT) + sizeof(DATA);
		fseek(m_pFile, 0, SEEK_SET);
		fwrite(&m_riff, sizeof(RIFF), 1, m_pFile);
		fwrite(&m_fmt,  sizeof(FMT),  1, m_pFile);
		fwrite(&m_data, sizeof(DATA), 1, m_pFile);

		fclose(m_pFile);
	}
}
