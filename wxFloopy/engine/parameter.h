#ifndef Parameter_H
#define Parameter_H


#include "floopyobj.h"
#include "objtypes.h"


#define SIZE_VALUE	0
#define SIZE_OFFSET	1
#define SIZE_ALL	2


class CRegion;
class CTrack;
class CTracks;
class CActionHistory;
class CParameters;


class CParameter : public IFloopyObj
{
//	DECLARE_DYNAMIC_CLASS(CParameter)

public:
	class CPoint : public IFloopyObj
	{
	public:
		CPoint(CParameter *parameter);
		CPoint(CPoint *pt);
		virtual ~CPoint() {}

		int GetType()	{ return FLOOPY_PARAMETER; }
		bool GetName(wxString &str);

		wxCursor GetCursor();

		void Move(int dx, int dy);
		void Refresh();

		void Select(bool selected=true);
		bool OnMouseEvent(wxMouseEvent& event);

		IFloopySoundInput *m_pInput;
		CParameter *m_pParameter;
		int		m_offset;
		int		m_samplesPerPixel;
		float	m_fScale;
		int		m_index;
		float	m_value;
		int		m_sizing;
		CActionHistory *m_pActionHistory;

		const char *GetLastErrorDesc() { return NULL; }
	};

public:
	CParameter(CParameters*, IFloopySoundInput*, int, bool, wxColor color=*wxCYAN);
	virtual ~CParameter();

	int GetType()	{ return FLOOPY_PARAMETER; }
	bool GetName(wxString &str);

	void Refresh();
	//void Invalidate();

	IFloopyObj *GetChildAt(int x, int y);
	void DrawFore(wxDC& dc, wxRect& rc);
//	float GetValueAt(int x);
	bool OnKeyDown(wxKeyEvent& event);
	bool OnMouseEvent(wxMouseEvent& event);
	void insertParam(int x, int y);
	bool HitTest(int x, int y);

	void Select(bool selected=true);
	void SelectPoint(CPoint *pt);

	IFloopyObj *GetSelectedObj();

	IFloopySoundInput *GetInput() { return m_pInput; }

	void Smooth(int start, int end, int res);

private:
	CRegion	*m_pRegion;
	CTrack	*m_pTrack;
	CTracks	*m_pTracks;
	int		m_index;
//	bool	m_bEdit;
//	int		m_iSamplePos, m_iPrevSamplePos;
	CPoint	*m_pPoint, *m_pSelectedPoint;
	IFloopySoundInput *m_pInput;
	bool	m_bAfterTrack;
	float	m_fScale;
	float	m_fMin, m_fMax;
	int		m_iSamplesPerPixel;
};


WX_DECLARE_LIST(CParameter, ParameterList);

class CParameters : public IFloopyObj
{
public:
	CParameters(IFloopyObj *parent);
	virtual ~CParameters();

	void LoadInput(IFloopySoundInput *obj);
	void RemoveInput(IFloopySoundInput *obj);
	void DeselectAll();

	void DeselectAll(CParameter *caller);

	void DrawFore(wxDC& dc, wxRect& rc);
	IFloopyObj *GetChildAt(int x, int y);
	IFloopyObj *GetSelectedObj();

	bool OnMouseEvent(wxMouseEvent& event);

	wxCursor GetCursor();

private:
	bool paramsLoaded(IFloopySoundInput *obj);
	bool isAfterTrack(IFloopySoundInput *obj);

private:
	ParameterList m_Parameters;
};


#endif // #ifndef Parameter_H
