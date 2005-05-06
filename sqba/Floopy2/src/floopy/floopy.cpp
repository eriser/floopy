// floopy.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "engine.h"
#include <assert.h>



#define BUFFER_LENGTH	5120 //512


void printPath(IFloopySoundInput *input, int level)
{
	if(!input)
		return;

	char space[100] = {0};
	if(level > 0)
	{
		int indent = level*2;
		for(int i=0; i<indent-2; i++)
			space[i] = ' ';
		
		space[i] = 0xb3;//0xc3;//0xc4;
		printf("%s\n", space);
		
		space[i] = 0xc0;//0xc3;//0xc4;
		
		space[i+1] = 0xc4;
		space[i+2] = 0xc4;
	}

	printf("%s%s\n", space, input->GetName());

//	if(level!=1 && input->GetInputCount() > 0)
//	{
	try {
		for(int i=0; i<input->GetInputCount(); i++)
		{
			printPath(input->GetSource(i), level+1);
		}
	}
	catch(...)
	{
		printf("This happens only in debug mode!!!\n");
	}
	// Zasto se ovo dogadja prilikom debagovanja i
	// zasto se sve stampa po dva puta???
/*	}
	else
	{
		printPath(input->GetSource(), level+1);
	}*/
}
/*
void printPath(IFloopySoundInput *input, IFloopySoundOutput *output)
{
	printf("%s\n", output->GetName());
	printPath(input, 1);
	getchar();
}
*/
void process(IFloopySoundInput *input, IFloopySoundOutput *output)
{
	clock_t start = clock();

	UINT offset = 0;
	BYTE buff[BUFFER_LENGTH];
	UINT len, size=sizeof(buff);
	memset(buff, 0, sizeof(buff));
//	while(((len=input->Read(buff, size, 0)) > 0) && (offset<1000000))
	while((len=input->Read(buff, size)) > 0)
	{
		offset += len;
		output->Write(buff, len);
		memset(buff, 0, sizeof(buff));
	}
	output->Close();

	DWORD speed = clock() - start;
	printf("Processing time: %d ms\n", speed);
	WAVFORMAT *fmt = input->GetFormat();
	assert((fmt->size > 0) && (fmt->channels > 0));
	//int x = ((fmt->size / 8) * fmt->channels);
	printf("Read:   %d samples\n", (offset / ((fmt->size / 8) * fmt->channels)));
	printf("Length: %f seconds\n", ((float)offset / (((float)fmt->size / 8.f) * (float)fmt->channels)) / (float)fmt->freq);
}

void main(int argc, char* argv[])
{
	CEngine *engine = new CEngine("engine");
	//engine->Open(TEXT("test.xml"));

	if(!engine->Open(TEXT("test.xml")))
		return;

	//COutput *output = NULL;
	IFloopySoundOutput *output = NULL;

	if(argc >= 2)
	{
		// Convert seconds to samples
		WAVFORMAT *fmt = engine->GetFormat();
		assert((fmt->size > 0) && (fmt->channels > 0));
		int x = fmt->freq;// * (fmt->size / 8) * fmt->channels;
		int i = atoi(argv[1]);
		engine->MoveTo(i*x);
	}
	if(argc >= 3)
	{
		// Convert seconds to samples
		WAVFORMAT *fmt = engine->GetFormat();
		assert((fmt->size > 0) && (fmt->channels > 0));
		int x = fmt->freq;// * (fmt->size / 8) * fmt->channels;
		int i = atoi(argv[2]);
		engine->SetSize(i*x);
	}

	WAVFORMAT *fmt = engine->GetFormat();
	assert((fmt->size > 0) && (fmt->channels > 0));

	int i = 0;

	switch(i)
	{
	case 0:
		// Render to file
		//output = new COutput(TEXT("wavfile"), engine);
		output = engine->CreateOutput("wavfile", fmt);
		output->Open("floopy.wav");
		break;
	case 1:
		// Output to speakers
		//output = new COutput(TEXT("waveout"), engine->getPlugin());
		output = engine->CreateOutput("waveout", fmt);
		break;
	case 2:
		// Output to svg
		output = engine->CreateOutput("svgfile", fmt);
		output->Open("floopy.svg");
		break;
	}

	// stdout?

//	printPath(engine, output);
	printf("%s\n", output->GetName());
	printPath(engine, 1);
	getchar();
//	WAVFORMAT *fmt = engine->getPlugin()->GetFormat();
	
	printf("%d samples\n", engine->GetSize());

	process(engine, output);

	delete output;
	delete engine;

	printf("Press enter to continue");
	getchar();
}

/*
void loadSection(IFloopyConfig *section)
{
}

int main(int argc, char* argv[])
{
	CStorage *storage = new CStorage(TEXT("storage"));
	COutput *output = NULL;

	IFloopyConfig *cfg = storage->getPlugin();

	if(cfg->Load(TEXT("config.xml")))
	{
		//IFloopyConfig *floopy = cfg->GetSection(TEXT("floopy"));
		IFloopyConfig *path = cfg->GetSection(TEXT("path"));
		IFloopyConfig *tmp = path->GetSection(TEXT("output"));
		if(tmp)
		{
			char *filename = tmp->GetName();
			tmp = tmp->GetSection(TEXT("input"));
			if(tmp)
			{
				char txt[100];
				tmp->GetProperty(TEXT("plugin"), txt, 100);
				CInput *input = new CInput(txt);
				output = new COutput( filename, input->getPlugin() );
			}
		}
	}

	delete storage;
	delete output;

	return 0;
}

int main(int argc, char* argv[])
{
	CInput *wavfilein1 = NULL;
	CInput *wavfilein2 = NULL;
	CInput *wavfilein3 = NULL;
	CInput *input = NULL;
	COutput *output = NULL;
	CInput *echo1 = NULL;
	CInput *echo2 = NULL;
	CInput *echo3 = NULL;
	CInput *mixer = NULL;
	CInput *loop = NULL;
	CInput *tonegen = NULL;
	CInput *volume = NULL;
	CInput *length = NULL;
	CInput *startat1 = NULL;
	CInput *startat2 = NULL;
//	CInput *view = NULL;

//	CStorage *storage = NULL;
//	storage = new CStorage(TEXT("storage"));

	//wavfilein1 = new CInput(argv[1]);
	wavfilein1 = new CInput(TEXT("wavfile"));
	wavfilein2 = new CInput(TEXT("wavfile"));
	wavfilein3 = new CInput(TEXT("wavfile"));

	echo1 = new CInput(TEXT("echo"));
	echo2 = new CInput(TEXT("echo"));
	echo3 = new CInput(TEXT("echo"));

	mixer = new CInput(TEXT("mixer"));

	loop = new CInput(TEXT("loop"));

	tonegen = new CInput(TEXT("tonegen"));

	volume = new CInput(TEXT("volume"));

	length = new CInput(TEXT("length"));

	startat1 = new CInput(TEXT("startat"));

	startat2 = new CInput(TEXT("startat"));

//	view = new CInput(TEXT("buffview"));

	if( wavfilein1->Open(TEXT("Ischlju.wav")) && 
		wavfilein2->Open(TEXT("presence.wav")) && 
		wavfilein3->Open(TEXT("Gimme Sh_T.wav")) )
	{
		IFloopySoundMixer *mxr = (IFloopySoundMixer*)mixer->getPlugin();

		loop->getPlugin()->SetParam(0, 8);
		loop->getPlugin()->SetSource(wavfilein1->getPlugin());

		//volume->getPlugin()->SetSource(loop->getPlugin());
		//mxr->AddSource(volume->getPlugin());
		mxr->AddSource(loop->getPlugin());

		startat1->getPlugin()->SetSource(wavfilein2->getPlugin());
		startat1->getPlugin()->SetParam(0, 44100.f * 3.f);
		mxr->AddSource(startat1->getPlugin());

		echo2->getPlugin()->SetSource(wavfilein3->getPlugin());
		mxr->AddSource(echo2->getPlugin());

		tonegen->getPlugin()->SetFormat(wavfilein1->getPlugin()->GetFormat());
		//mxr->AddSource(echo3->getPlugin());
		tonegen->getPlugin()->SetParam(0, 1800.f);

		//length->getPlugin()->SetParam(0, 300000.f);
		length->getPlugin()->SetSource(tonegen->getPlugin());
		length->getPlugin()->SetParam(0, 44100.f);
		startat2->getPlugin()->SetSource(length->getPlugin());
		startat2->getPlugin()->SetParam(0, 44100.f*2.f);
		mxr->AddSource(startat2->getPlugin());


		output = new COutput(TEXT("wavfile"), mxr);
		output->Open(TEXT("out.wav"));
		//output = new COutput(TEXT("waveout"), mxr);


		echo1->getPlugin()->SetSource(mixer->getPlugin());
		//echo3->getPlugin()->SetSource(echo2->getPlugin());

		//input = wavfile;
		input = echo1;
		//input = mixer;
		//input = wavfilein1;

		//printf("Press enter to start");
		//getchar();

//		view->getPlugin()->SetSource(input->getPlugin());

		process(input, output);

		mxr->Close();
	
		//printf("Press enter to continue");
		//getchar();
	}

	return 0;
}
*/
