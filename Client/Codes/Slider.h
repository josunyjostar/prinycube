#ifndef Slider_h__
#define Slider_h__

#include "UI.h"
#include "SliderHandle.h"

class CSlider : public CUI
{
protected:
	explicit CSlider();
	virtual ~CSlider();

public:
	DeclRTTI(CSlider, CUI);

	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Render();

private:
	RCP_SLIDERHANDLE	m_pSliderHandle;
	float				m_fMinVal;
	float				m_fMaxVal;
	float				m_fVal;

public:
	CREATE(CSlider);
};

TYPEDEF_RCP(CSlider, SLIDER);

#endif // Slider_h__
