#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {


protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
};
 
class MainWindow : public Window {
	LOGFONT lf;
	COLORREF col;

protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
