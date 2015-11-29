#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {
public:
	int n;

protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();

};
 
class MainWindow : public Window {
	LOGFONT lf;
	COLORREF col;
	int maxNum;
public:
	MainWindow() : maxNum(10), col(RGB(0, 0, 0)) {
		ZeroMemory(&lf, sizeof lf);
		_tcscpy(lf.lfFaceName, _T("Arial"));
		HDC hdc = GetDC(0);
		lf.lfHeight = -12 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
		ReleaseDC(0, hdc);
	}
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
