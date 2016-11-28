#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {
public:
	int req_number; //requested number
protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
};
 
class MainWindow : public Window {
private:
	LOGFONT lf;
	COLORREF color;
	int max_number;
public:
	MainWindow() : max_number(15), color(RGB(0, 0, 0)) {
		ZeroMemory(&lf, sizeof(lf));
		_tcscpy(lf.lfFaceName,_T("Arial"));
		HDC hdc = GetDC(0);
		lf.lfHeight = -16 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
		ReleaseDC(0, hdc);
	}
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};

bool make_font(LOGFONT &lf, COLORREF &color) {
	CHOOSEFONT cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.lStructSize = sizeof(cf);
	cf.rgbColors = color;
	cf.lpLogFont = &lf;
	cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	if (ChooseFont(&cf)) { color = cf.rgbColors; return true; } 
	return false;
}