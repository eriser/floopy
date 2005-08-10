#ifndef _FLOOPYFRAME_H
#define _FLOOPYFRAME_H


#include <wx/dnd.h>
#include <wx/splitter.h>

#include "views/labelsview.h"
#include "views/tracksview.h"


class CFloopyFrame : public wxFrame
{
//	DECLARE_DYNAMIC_CLASS(CFloopyFrame)

	class CDropTarget : public wxFileDropTarget
	{
	public:
		CDropTarget(CFloopyFrame *frm) { m_pFrame = frm; }
		virtual ~CDropTarget() {}

		bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
		{
			for(int i=0; i<(int)filenames.Count(); i++)
			{
				m_pFrame->Open( (char*)filenames.Item(i).GetData() );
			}
			return TRUE;
		}

	private:
		CFloopyFrame *m_pFrame;
	};

public:
	/**
	 * Constructor. Creates a new TextFrame
	 */
	CFloopyFrame(const wxChar *title, int xpos, int ypos, int width, int height);

	/**
	 * Destructor
	 */
	~CFloopyFrame();

//	bool OnCreate(wxDocument *doc, long WXUNUSED(flags));
//	bool OnClose(bool deleteWindow = true);
	void OnMouseEvent(wxMouseEvent& event);
	void OnFullScreen( wxCommandEvent &WXUNUSED(event) );
    void OnQuit(wxCommandEvent &WXUNUSED(event));
    void OnAbout(wxCommandEvent &WXUNUSED(event));
    void OnFileOpen(wxCommandEvent &WXUNUSED(event));
    void OnFileSave(wxCommandEvent &WXUNUSED(event));
    void OnFileClose(wxCommandEvent &WXUNUSED(event));
	void OnPlay( wxCommandEvent &WXUNUSED(event) );
	void OnPause( wxCommandEvent &WXUNUSED(event) );
	void OnStop( wxCommandEvent &WXUNUSED(event) );

	void Open(char *filename);

private:
	void initMenus();
	void initViews();

private:
	CDropTarget			*m_pDropTarget;
	wxSplitterWindow	*m_pSplitter;
	CLabelsView			*m_pLabelsView;
	CTracksView			*m_pTracksView;
	CTracks				*m_pTracks;

private:
    DECLARE_EVENT_TABLE()
};

#endif _FLOOPYFRAME_H
