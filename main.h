#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {
public:
	int uneseni_broj;
protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
};
 
class MainWindow : public Window {
private:
	LOGFONT lf;
	COLORREF color;
	int max_broj;
public:
	MainWindow() : max_broj(15) {
		ZeroMemory(&lf, sizeof(lf));
		_tcscpy(lf.lfFaceName, _T("Arial"));
		HDC hdc = GetDC(0);
		lf.lfHeight = -16 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
		ReleaseDC(0, hdc);
	}

protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
