#ifndef SliderHandle_h__
#define SliderHandle_h__

#include "Button.h"

class CSliderHandle : public CButton
{
protected:
	explicit CSliderHandle():m_bClick(false) {}
	virtual ~CSliderHandle() {}

public:
	DeclRTTI(CSliderHandle, CButton);

	bool IsClick() const { return m_bClick; }

public:
	virtual void Update();
	virtual void OnMouseButtonDown()	{ m_bClick = true; }
	virtual void OnMouseButtonUp()		{ m_bClick = false; }

private:
	bool m_bClick;

public:
	CREATE(CSliderHandle);
};

TYPEDEF_RCP(CSliderHandle, SLIDERHANDLE);

#endif // SliderHandle_h__
