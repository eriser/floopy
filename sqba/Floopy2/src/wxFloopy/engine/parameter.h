#ifndef Parameter_H
#define Parameter_H


#include "floopyobj.h"
#include "ObjTypes.h"


#define SIZE_VALUE	0
#define SIZE_OFFSET	1
#define SIZE_ALL	2


class CRegion;


class CParameter : public IFloopyObj
{
//	DECLARE_DYNAMIC_CLASS(CParameter)

public:
	class CPoint : public IFloopyObj
	{
	public:
		CPoint(CParameter *parameter) : IFloopyObj(parameter) { m_offset = 0; }
		virtual ~CPoint() {}

		wxCursor GetCursor();

		void Move(int dx, int dy);

		int m_offset;
		int m_samplesPerPixel;
		float m_fScale;
		IFloopySoundInput *m_pObj;
		int m_index;
		float m_value;
		int m_iSizeOrientation;
	};

public:
//	CParameter() {}
	CParameter(CRegion *region, IFloopySoundInput *obj, int index, bool bAfterTrack, wxColor color=*wxCYAN);
	virtual ~CParameter();

	int GetType()	{ return FLOOPY_PARAMETER; }

	IFloopyObj *GetChildAt(int x, int y);
	float GetValueAt(int x);
	void DrawFore(wxDC& dc, wxRect& rc);
//	void DrawBG(wxDC& dc, wxRect& rc);
	bool HitTest(int x, int y);

private:
	inline CRegion	*getRegion()	{ return (CRegion*)GetParent(); }
	inline CTrack	*getTrack();
	inline CTracks	*getTracks();

private:
	int		m_index;
	bool	m_bEdit;
	int		m_iSamplePos, m_iPrevSamplePos;
	CPoint	*m_pPoint;
	IFloopySoundInput *m_pObj;
	bool m_bAfterTrack;
	float m_fScale;
};


#endif // #ifndef Parameter_H
