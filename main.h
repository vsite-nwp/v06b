#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {
public:
	int requestNum;
protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
};
 
class MainWindow : public Window {
	LOGFONT lf;
	int maxNum;
public:
	MainWindow(): maxNum(5)
	{
		::ZeroMemory(&lf, sizeof(lf));
		_tcscpy(lf.lfFaceName, _T("Arial"));
		HDC hdc = GetDC(0);
		lf.lfHeight = -18*GetDeviceCaps(hdc, LOGPIXELSY) / 72;
		ReleaseDC(0, hdc);
	}
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
