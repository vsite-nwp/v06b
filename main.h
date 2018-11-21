#include "nwpwin.h"
#include "nwpdlg.h"


class NumberDialog : public Dialog {
public:
	int num;
protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
};
 
class MainWindow : public Window {
	LOGFONT lf = { 0 };
	int MaxNum;
public:
 MainWindow() : MaxNum(16)
 {
	 strcpy(lf.lfFaceName, "Arial");
	 HDC h = GetDC(0);
	 lf.lfHeight = -12 * GetDeviceCaps(h, LOGPIXELSX)/72 ;
	 ReleaseDC(0, h);
 }
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
