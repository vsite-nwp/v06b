#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {
protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
public:
	int newMax;
};
 
class MainWindow : public Window {
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
public:
	MainWindow();
	int maxNumber;
	LOGFONT lfont;
	
};
