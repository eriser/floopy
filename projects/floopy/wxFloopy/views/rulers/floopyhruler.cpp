// FloopyHRuler.cpp: implementation of the CFloopyHRuler class.
//
//////////////////////////////////////////////////////////////////////

#include "../../globals.h"
#include "floopyhruler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFloopyHRuler::CFloopyHRuler(CRulerView *parent, CTracks *tracks)
 : CHorizontalRuler( parent )
{
	m_pTracks = tracks;

	SetWindowStyle(wxNO_BORDER);

	m_bDrawAquaBG = DRAW_AQUA_RULERS;
}

CFloopyHRuler::~CFloopyHRuler()
{

}


void CFloopyHRuler::OnDrawBG(wxDC &dc)
{
	// Background and frame
	wxSize size = this->GetClientSize();
	int left=0, top=0;
/*
	int xScrollUnits=0, yScrollUnits=0;
	m_owner->GetViewStart(&left, &top);
	m_owner->GetScrollPixelsPerUnit( &xScrollUnits, &yScrollUnits );
	left *= xScrollUnits;
	int width = m_owner->GetClientSize().GetWidth();
*/
	//int width = size.GetWidth();
	int width = m_pTracks->GetWidth();
	int height = size.GetHeight();

	if(m_bDrawAquaBG)
	{
	    wxRect rc(0, 0, width, height);
		DrawAquaRect(dc, rc , 0);
	}
	else
	{
	    wxRect rc(left, 0, width, height);
		DrawRect3D(dc, rc);
	}
}
