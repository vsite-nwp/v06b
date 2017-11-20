#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {
public:
	int edNum;
protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
};
 
class MainWindow : public Window {
	LOGFONT font;
	int numNumbers;
	COLORREF fontColor;
public:
	MainWindow();
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
