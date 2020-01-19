#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {
public:
	int number;

protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
};
 
class MainWindow : public Window {
	LOGFONT logFont;
	COLORREF colorRef;
	int maxNumber;
public:
	MainWindow() : maxNumber(10), colorRef(RGB(0, 0, 0)) {
		ZeroMemory(&logFont, sizeof logFont);
		_tcscpy(logFont.lfFaceName, _T("Aria"));
		HDC hdc = GetDC(0);
		logFont.lfHeight = -12 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
		ReleaseDC(0, hdc);
	}
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
