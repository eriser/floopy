
//#include <string.h>
#include "dump.h"


CDump::CDump()
{
	m_fp = stdout;
	m_iLevel = 0;
}

CDump::~CDump()
{

}

void CDump::dump_properties(IFloopyProperty *obj)
{
	for(int i=0; i<obj->GetPropertyCount(); i++)
	{
		const char *name = obj->GetPropertyName(i);
		const char *desc = obj->GetPropertyDesc(i);
		const char *unit = obj->GetPropertyUnit(i);

		float value = 0.f;
		obj->GetPropertyVal(i, &value);

		float min = obj->GetPropertyMin(i);
		float max = obj->GetPropertyMax(i);
		float step = obj->GetPropertyStep(i);

		dump_indentation();
		fprintf(m_fp, "%s, %s, %s, %f, %f, %f, %f\n", name, desc, unit, value, min,  max, step);
	}
}

void CDump::dump_parameters(IFloopyParam *obj)
{
	for(int i=0; i<obj->GetParamCount(); i++)
	{
		const char *name = obj->GetParamName(i);
		const char *desc = obj->GetParamDesc(i);
		const char *unit = obj->GetParamUnit(i);

		float value = 0.f;
		obj->GetParamVal(i, &value);

		float min = obj->GetParamMin(i);
		float max = obj->GetParamMax(i);
		float step = obj->GetParamStep(i);

		bool bEnabled = obj->IsEnabled();

		dump_indentation();
		fprintf(m_fp, "%s, %s, %s, %f, %f, %f, %f, %s\n", name, desc, unit, value, min,  max, step, bEnabled?"enabled":"disabled");
	}
}


void CDump::dump_timeline(IFloopyTimeline *obj, int param)
{
	int offset = 0;
	float value = 0.f;
	dump_indentation();
	fprintf(m_fp, "Parameter %d: ", param);
	while( true )
	{
		offset = obj->GetNextOffset(offset, param);
		if( offset <= 0 )
			break;

		if( !obj->GetParamAt(offset, param, &value) )
			break;

		fprintf(m_fp, "%d:%f", offset, value);
	}
	fprintf(m_fp, "\n");
}

void CDump::dump_timeline(IFloopyTimeline *obj)
{
	for(int i=0; i<obj->GetParamCount(); i++)
	{
		dump_timeline(obj, i);
	}
}

void CDump::dump_info(IFloopyDisplay *obj)
{
	int type = obj->GetType();
	const char *name = obj->GetName();
	const char *desc = obj->GetDescription();
	const char *ver = obj->GetVersion();
	const char *auth = obj->GetAuthor();

	dump_indentation();
	fprintf(m_fp, "%s, %s, %s, %s\n", name, desc, ver, auth);
}

void CDump::dump_inputs(IFloopySoundMixer *obj)
{
	for(int i=0; i<obj->GetInputCount(); i++)
	{
		IFloopySoundInput *src = obj->GetSource(i);
		dump_input( src );
	}
}

void CDump::dump_input(IFloopySoundInput *obj)
{
	dump_info( obj );
	dump_properties( obj );
	dump_parameters( obj );
	dump_timeline( obj );
	m_iLevel++;
	if(	is_mixer(obj) )
		dump_inputs( (IFloopySoundMixer*)obj );
	else if( is_filter(obj) )
		dump_input( ((IFloopySoundFilter*)obj)->GetSource() );
	m_iLevel--;
}

bool CDump::is_filter(IFloopySoundInput *input)
{
	int type = input->GetType();
	return(type == (TYPE_FLOOPY_SOUND_FILTER | type));
}

bool CDump::is_mixer(IFloopySoundInput *input)
{
	int type = input->GetType();
	return(type == (TYPE_FLOOPY_SOUND_MIXER | type));
}

void CDump::dump_indentation()
{
	char *space = new char[m_iLevel*2+1];
//	strnset(space, ' ', m_iLevel*2);
	memset(space, ' ', m_iLevel*2);
	space[m_iLevel*2] = 0;
	fprintf(m_fp, "%s", space);
	delete [] space;
}

bool CDump::Save(IFloopySoundEngine *engine, const char *filename)
{
	m_iLevel = 0;
	m_fp = filename ? fopen(filename, "wt") : stdout;
	dump_input( engine );
	if( filename &&m_fp )
		fclose( m_fp );
	return true;
}
