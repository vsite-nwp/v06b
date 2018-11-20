#include "nwpwin.h"
#include "nwpdlg.h"

class NumberDialog : public Dialog {
protected:
	int IDD();
	bool OnInitDialog();
	bool OnOK();

public:
	int MaxNumber = 10;
};
 
class MainWindow : public Window {
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();

public:
	int MaxNumber;
	LOGFONT Font;
	MainWindow();
};
