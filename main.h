#include "nwpwin.h"
#include "nwpdlg.h"


class NumberDialog : public Dialog {
public:
	int broj;
protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();
};
 
class MainWindow : public Window {
public:
	int broj;	
	LOGFONT lf;
	MainWindow(int broj) : broj(broj)
	{
		lf = { 0 };
		strcpy(lf.lfFaceName, "Arial");
	};
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
