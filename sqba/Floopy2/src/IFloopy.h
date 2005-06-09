#ifndef _IFLOOPY_H_
#define _IFLOOPY_H_


#include <memory.h>	// for memset()

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <windows.h>



#define TIMELINE_PARAM_ENABLE		-1
#define PARAM_VALUE_ENABLED		1000.f
#define PARAM_VALUE_DISABLED	2000.f
#define TIMELINE_PARAM_MOVETO		-2


typedef int                 INT;
typedef int                 BOOL;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
typedef unsigned long       DWORD;

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif


//#define EOF     (-1)	// defined in stdio

/*********************************************************************
 *! \struct SoundFormat
 *  \brief Sound format information.
 *  \author Filip Pavlovic
 *  \version 0.0
 *  \date 13. April 2005.
 *********************************************************************/
typedef struct SoundFormat
{
	INT		frequency;		/** Audio frequency (samples per second)*/
	WORD	format;			/** Audio data format					*/
	BYTE	channels;		/** Number of channels: 1-mono, 2-stereo*/
	DWORD	bitsPerSample;	/** Sample size, in bits				*/
//	INT		format;			/** Waveform-audio format type			*/
} SOUNDFORMAT;


/*********************************************************************
 *! \enum enumClassType
 *  \brief Runtime class information.
 *  \author Filip Pavlovic
 *  \version 0.0
 *  \date 30. May 2005.
 *
 *  Returned by IFloopy::GetType().
 *********************************************************************/
enum enumClassType
{
	TYPE_FLOOPY = 0,			/** IFloopy				*/
	TYPE_FLOOPY_SOUND_INPUT,	/** IFloopySoundInput	*/
	TYPE_FLOOPY_SOUND_OUTPUT,	/** IFloopySound		*/
	TYPE_FLOOPY_ENGINE			/** IFloopySoundEngine	*/
};


/*********************************************************************
 *! \class IFloopyCallback
 *  \brief Main callback interface.
 *  \author Filip Pavlovic
 *  \version 0.0
 *  \date 13. April 2005.
 *
 *  Passed to all components.
 *********************************************************************/
/*class IFloopyCallback
{
public:
	IFloopyCallback()	{ }
	virtual ~IFloopyCallback()	{ }

	void OnParameterUpdate(IFloopy *src, int index)
};*/


/*********************************************************************
 *! \class IFloopy
 *  \brief Main object interface.
 *  \author Filip Pavlovic
 *  \version 0.0
 *  \date 13. April 2005.
 *********************************************************************/
class IFloopy
{
public:
	IFloopy()			{ m_bEnabled = TRUE; m_nLastError = 0; }
	virtual ~IFloopy()	{ }

	/**
	 * Returns class type identificator.
	 * Used for runtime class identification.
	 * Do not override in implementations!!!
	 * @return class type identificator.
	 */
	virtual enumClassType GetType()	{ return TYPE_FLOOPY; }

	// Component description
	virtual char *GetName()			{ return "IFloopy"; }
	virtual char *GetDescription()	{ return "IFloopy interface"; }
	virtual char *GetVersion()		{ return "0.1"; }
	virtual char *GetAuthor()		{ return "sqba"; }

	// Component parameters
	virtual int   GetParamCount()					{ return 0; }
	virtual void  SetParam(int index, float value)	{ }
	virtual float GetParam(int index)				{ return 0.f; }
	virtual float GetParamMin(int index)			{ return 0.f; }
	virtual float GetParamMax(int index)			{ return 0.f; }
	virtual char *GetParamName(int index)			{ return NULL; }
	virtual char *GetParamDesc(int index)			{ return NULL; }
	virtual BOOL  GetParam(int index, float *value)	{ return FALSE; }
	virtual int   GetParamIndex(char *name)			{ return -1; }

	virtual void  Enable(BOOL bEnabled)		{ m_bEnabled = bEnabled; }
	virtual BOOL  IsEnabled()				{ return m_bEnabled; }

			int   GetLastError()			{ return m_nLastError; }
	virtual char *GetLastErrorDesc()		{ return NULL; }
	//virtual BOOL  GetLastError(char *str, int len)	{ return FALSE; }

	/** Do not override in implementations, handled by the engine */
	virtual char *GetDisplayName()					{ return NULL; }

	/** Do not override in implementations, handled by the engine */
	virtual void  SetDisplayName(char *name, int len){ }

	/** Do not override in implementations, handled by the engine */
	virtual char *GetPath()							{ return NULL; }

	virtual BOOL Open(char *filename)				{ return FALSE; }
	virtual void Close()							{ }

	/**
	 * Returns next sample position at which a parameter change occurs.
	 * Do not override in implementations, handled by the engine.
	 * @param offset current position, in samples.
	 * @return next position (number of samples).
	 */
	virtual int GetNextOffset(int offset)			{ return 0; }

	/**
	 * Schedules particular parameter's value for change at specific offset.
	 * Do not override in implementations, handled by the engine.
	 * @param offset position in samples at which the change occurs.
	 * @param index index of the parameter.
	 * @param value new parameter value.
	 */
	virtual void SetParamAt(int offset, int index, float value) {}

	/**
	 * Removes scheduled parameter change.
	 * Do not override in implementations, handled by the engine.
	 * @param offset position in samples at which the change was supposed to occur.
	 * @param index index of the parameter.
	 */
	virtual BOOL ResetParamAt(int offset, int index) { return FALSE; }

	/**
	 * Enables/disables object at specific offset.
	 * Do not override in implementations, handled by the engine.
	 * @param offset position in samples at which the change was supposed to occur.
	 * @param bEnable enable or disable.
	 */
	virtual void EnableAt(int offset, BOOL bEnable) {}

private:
	BOOL m_bEnabled;	/** Is this component disabled or enabled. */

protected:
	int m_nLastError;	/** Last error code. */
};


/*********************************************************************
 *! \class IFloopySoundInput
 *  \brief Main sound input interface.
 *  \author Filip Pavlovic
 *  \version 0.0
 *  \date 13. april 2005
 *
 *  Interface implemented and used by all sound input objects
 *  such as sound source objects and filter (effect) objects.
 *********************************************************************/
class IFloopySoundInput : public IFloopy
{
public:
	IFloopySoundInput() : IFloopy()
	{
		m_source = NULL;
		memset(&m_format, 0, sizeof(SOUNDFORMAT));
		//m_pos = 0;
	}
	virtual ~IFloopySoundInput() {}

	enumClassType GetType()	{ return TYPE_FLOOPY_SOUND_INPUT; }

	// Component description
	char *GetName()			{ return "IFloopySoundInput"; }
	char *GetDescription()	{ return "IFloopySoundInput interface"; }

	/**
	 * Open source file.
	 */
	BOOL Open(char *filename)
	{
		return (NULL != m_source ? m_source->Open(filename) : FALSE);
	}

	/**
	 * Return total sample count.
	 * @return number of samples.
	 */
	virtual int GetSize()
	{
		return (NULL != m_source ? m_source->GetSize() : 0);
	}

	/**
	 * Return total play size, in samples.
	 * @return number of samples.
	 */
	virtual int GetLength()
	{
		return (NULL != m_source ? m_source->GetLength() : 0);
	}

	virtual BOOL SetSource(IFloopySoundInput *src)
	{
		m_source = src;
		return TRUE;
	}

	virtual IFloopySoundInput *GetSource()
	{
		return m_source;
	}

	/**
	 * Fills given buffer with sound data.
	 * @param data pointer to the buffer to be filled.
	 * @param size size of the buffer in bytes.
	 * @param offset number of bytes to skip.
	 * @return number of bytes read from data.
	 */
	virtual int Read(BYTE *data, int size)
	{
		//int len = (NULL != m_source ? m_source->Read(data, size) : 0);
		//m_pos += len;
		//return len;
		return (NULL != m_source ? m_source->Read(data, size) : 0);
	}

	/**
	 * Moves the starting position.
	 * @param samples number of samples to move from the start.
	 */
	virtual void MoveTo(int samples)
	{
		if(NULL != m_source)
			m_source->MoveTo(samples);
		//SOUNDFORMAT *fmt = GetFormat();
		//m_pos = samples * ( (fmt->size / 8) * fmt->channels );
	}

	/**
	 * Returns current sample position.
	 */
	virtual int GetPos()
	{
		return (NULL != m_source ? m_source->GetPos() : 0);
		//	return m_pos;
	}

	/**
	 * Moves to the starting position.
	 */
	virtual void Reset()
	{
		if(NULL != m_source)
			m_source->Reset();
		//m_pos = 0;
	}

	virtual SOUNDFORMAT *GetFormat()
	{
		return (NULL != m_source ? m_source->GetFormat() : &m_format);
	}

	/**
	 * Mixer specific function.
	 */
	virtual int AddSource(IFloopySoundInput *src)
	{
		return (NULL != m_source ? m_source->AddSource(src) : 0);
	}

	/**
	 * Mixer specific function.
	 */
	virtual IFloopySoundInput *GetSource(int index)
	{
		return (index > 0 ? NULL : m_source);
	}

	/**
	 * Mixer specific function.
	 */
	virtual void RemoveSource(IFloopySoundInput *src)
	{
		//if(NULL != m_source)
		//	m_source->RemoveSource(src);
	}

	/**
	 * Mixer specific function.
	 */
	virtual int GetInputCount()
	{
		return (NULL != m_source ? 1 : 0);
	}

	/**
	 * Specifies whether the source should be read and passed on 
	 * if this component is disabled. If false then nothing
	 * will be read.
	 */
	virtual BOOL ReadSourceIfDisabled()	{ return TRUE; }

	// Utility
	/*int SamplesToBytes(int samples)
	{
		SOUNDFORMAT *fmt = GetFormat();
		return samples * ( (fmt->bitsPerSample / 8) * fmt->channels );
	}*/

protected:
	IFloopySoundInput *m_source;
	SOUNDFORMAT m_format;
	//int m_pos;
};

/*********************************************************************
 *! \class IFloopySoundOutput
 *  \brief Main sound output interface.
 *  \author Filip Pavlovic
 *  \version 0.0
 *  \date 13. april 2005
 *
 *  Interface implemented and used by all sound output objects
 *  such as file writers and encoders.
 *********************************************************************/
class IFloopySoundOutput : public IFloopy
{
public:
	IFloopySoundOutput() : IFloopy() { m_dest = NULL; }
	IFloopySoundOutput(SOUNDFORMAT fmt) { m_dest = NULL; }
	virtual ~IFloopySoundOutput() {}

	enumClassType GetType()	{ return TYPE_FLOOPY_SOUND_OUTPUT; }

	char *GetName()			{ return "IFloopySoundOutput"; }
	char *GetDescription()	{ return "IFloopySoundOutput interface"; }

	/**
	 * Opens destination (file).
	 */
	BOOL Open(char *filename)
	{
		return (NULL != m_dest ? m_dest->Open(filename) : FALSE);
	}

	/**
	 * Closes destination (file).
	 */
	void Close()
	{
		if(NULL != m_dest)
			m_dest->Close();
	}

	/**
	 * Writes a buffer to the output, whatever it may be.
	 * @param size size of the data buffer in bytes.
	 * @param data pointer to the buffer to be written into output.
	 * @return number of bytes read from data into output.
	 */
	virtual int Write(BYTE *data, int size)
	{
		return (NULL != m_dest ? m_dest->Write(data, size) : 0);
	}

	virtual void SetDest(IFloopySoundOutput *dst)	{ m_dest = dst; }
	virtual IFloopySoundOutput *GetDest()			{ return m_dest; }

private:
	IFloopySoundOutput *m_dest;
};


typedef void (*UpdateCallback)(IFloopy *src, int offset, int param);


/*********************************************************************
 *! \class IFloopyEngine
 *  \brief Floopy engine interface.
 *  \author Filip Pavlovic
 *  \version 0.0
 *  \date 13. april 2005
 *
 *  Floopy engine is the main object responsible for creation and
 *  destruction of plugins, as well as all parameter changes on
 *  the timeline.
 *********************************************************************/
class IFloopySoundEngine : public IFloopySoundInput
{
public:
	enumClassType GetType() { return TYPE_FLOOPY_ENGINE; }

	char *GetName()			{ return "IFloopySoundEngine"; }
	char *GetDescription()	{ return "IFloopySoundEngine interface"; }

	/**
	 * Creates a sound source or filter.
	 * @param name Component name (without extension).
	 */
	virtual IFloopySoundInput  *CreateInput(char *name)
	{ return NULL; }

	/**
	 * Creates a sound output.
	 * @param name Component name (without extension).
	 */
	virtual IFloopySoundOutput *CreateOutput(char *name, SOUNDFORMAT fmt)
	{ return NULL; }

	/**
	 * Destroys the object created by CreateInput() or CreateOutput().
	 * @param obj Pointer to the object being deleted.
	 */
	virtual void DeleteObject(IFloopy *obj) {}

	/**
	 * Serializes current engine state into a file.
	 * @param filename Project file name.
	 */
	virtual BOOL Save(char *filename) { return FALSE; }

	/**
	 * Sets the callback function that is called on parameter changes.
	 * @param func Pointer to the callback function.
	 */
	virtual void RegisterUpdateCallback(UpdateCallback func) {}
};


#endif //_IFLOOPY_H_
