// LabelsView.h: interface for the CLabelsView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCANVAS_H__8E37300D_BB37_4B3D_A520_0278F7DAA9B9__INCLUDED_)
#define AFX_MYCANVAS_H__8E37300D_BB37_4B3D_A520_0278F7DAA9B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __GNUG__
// #pragma implementation
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include <wx/docview.h>
#include <wx/log.h>

#include "../engine/tracks.h"

class CLabelsView : public wxScrolledWindow
{
public:
	CLabelsView(wxWindow* parent, CTracks *tracks);

	void OnDraw(wxDC& dc);
	void OnMouseEvent(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	//void OnPaint( wxPaintEvent &event );
//	void ScrollWindow( int dx, int dy, const wxRect *rect );

	//void Scroll(int x, int y);
	void OnSize(wxSizeEvent& event);

	void SetTracksView(wxScrolledWindow *panel) { m_pTracksView = panel; }

private:
	//int m_yScrollUnits;
//	int m_yOrigin;
	CTracks *m_tracks;
	wxScrolledWindow *m_pTracksView;

private:
    DECLARE_EVENT_TABLE()
};

#endif // !defined(AFX_MYCANVAS_H__8E37300D_BB37_4B3D_A520_0278F7DAA9B9__INCLUDED_)
