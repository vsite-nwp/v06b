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
	MainWindow() : maxNumber(10), colorRef(RGB(0, 0, 0)) {	}
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
