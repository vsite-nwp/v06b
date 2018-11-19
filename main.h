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
 MainWindow() : MaxNum(16){}
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
