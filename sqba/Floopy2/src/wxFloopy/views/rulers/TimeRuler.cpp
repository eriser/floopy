// TimeRuler.cpp: implementation of the CTimeRuler class.
//
//////////////////////////////////////////////////////////////////////

#include "TimeRuler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeRuler::CTimeRuler(CRulerView *parent, CTracks *tracks)
 : CFloopyHRuler( parent, tracks )
{

}

CTimeRuler::~CTimeRuler()
{

}

void CTimeRuler::OnDraw(wxDC &dc)
{
//	int left=0, top=0;
//	int xScrollUnits=0, yScrollUnits=0;
//	m_owner->GetViewStart(&left, &top);
//	m_owner->GetScrollPixelsPerUnit( &xScrollUnits, &yScrollUnits );
//	left *= xScrollUnits;


	// Background and frame
	wxSize size = this->GetClientSize();
	//int width = m_pTracks->GetWidth();
	//int height = size.GetHeight();
	//DrawRect3D(dc, wxRect(0, 0, width, height));
	CFloopyHRuler::OnDraw(dc);

	int pix      = m_pTracks->GetPixelsPerSecond();
	int iMiddle  = size.GetHeight()/2;
	int iStep    = m_pTracks->CalcStep( MIN_DISTANCE );

	int iLineTop1 = iMiddle + 1;
	int iLineTop2 = iMiddle + (iMiddle/3)*2;

	int iLineTop = 0;
	int iLineBottom = size.GetHeight();

	int right = m_pTracks->GetWidth();

	dc.SetFont( *wxSWISS_FONT );

	int w=0, h=0;
	wxString csLabel("00:00:000");
	dc.GetTextExtent(csLabel, &w, &h);
	int iTextTop = iLineTop1 - (h - h/3);

	for(int x=0; x<right; x+=iStep)
	{
		//if((x/iStep)%4 == 0)
		//Optimization: x%a == x&(a-1) for binary numbers
		//if(x>=left && ((x/iStep)&3) == 0)
		if(((x/iStep)&3) == 0)
		{
			iLineTop = iLineTop1;

			if(x+4+w < right)
			{
				float fSeconds = (float)x / (float)pix;
				FormatTime(fSeconds, csLabel);
				dc.DrawText(csLabel, x+4, iTextTop);
			}
		}
		else
		{
			iLineTop = iLineTop2;
		}


		dc.SetPen(*wxBLACK_PEN);
		dc.DrawLine(x, iLineTop, x, iLineBottom);

		dc.SetPen(*wxWHITE_PEN);
		dc.DrawLine(x+1, iLineTop, x+1, iLineBottom);
	}
}
