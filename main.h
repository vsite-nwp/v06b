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
	NumberDialog ndl;
	LOGFONT lf;	
	HFONT hFont = NULL;
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
