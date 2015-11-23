#include "nwpwin.h"
#include "nwpdlg.h"
const int default_broj = 11;

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
	MainWindow()
	{
		broj = default_broj;
		lf.lfHeight = -17; 
		lf.lfWidth = 0;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfItalic = '\0';
		lf.lfUnderline = '\0';
		lf.lfStrikeOut = '\0';
		lf.lfCharSet = '\0';
		lf.lfClipPrecision = '\X2';
		lf.lfQuality = '\X1';
		lf.lfWeight = 400;
		lf.lfOutPrecision = '\X3';
		lf.lfPitchAndFamily = 34;
		strcpy(lf.lfFaceName, "Arial");
	};
protected:
	void OnPaint(HDC hdc);
	void OnCommand(int id);
	void OnDestroy();
};
