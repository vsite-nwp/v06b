#include "main.h"
#include "rc.h"


int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, number);
	return true;
}
bool NumberDialog::OnOK(){
	try {
		number = GetInt(IDC_EDIT1);
	}
	catch (XCtrl&){
		MessageBox(*this, _T("Works for the decadal numbering system only!"), NULL, MB_OK);
		return false;
	}
	return true;
}

MainWindow::MainWindow() :maxNumber(10) {
	colorRef = RGB(0, 0, 0);
	ZeroMemory(&logFont, sizeof logFont);
	_tcscpy(logFont.lfFaceName, _T("Arial"));
	HDC hdc = GetDC(0);
	logFont.lfHeight = -10 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	ReleaseDC(0, hdc);

}
 
void MainWindow::OnPaint(HDC hdc){
	RECT rct; GetClientRect(*this, &rct);
	POINT pnt{ rct.right / (maxNumber + 1), rct.bottom / (maxNumber + 1) };

	HFONT currFont = CreateFontIndirect(&logFont);
	HFONT oldFont = (HFONT)SelectObject(hdc, currFont);
	MoveToEx(hdc, pnt.x, 0, 0);
	LineTo(hdc, pnt.x, rct.bottom);
	MoveToEx(hdc, 0, pnt.y, 0);
	LineTo(hdc, rct.right, pnt.y);
	SetTextColor(hdc, colorRef);

	for (int cntRow = 1; cntRow <= maxNumber + 1; ++cntRow) {
		for (int cntClmn = 1; cntClmn <= maxNumber + 1; ++cntClmn) {
			RECT loopRect{ cntRow * pnt.x, cntClmn * pnt.y, (cntRow + 1) * pnt.x, (cntClmn + 1) * pnt.y };
			std::string mult = std::to_string(cntRow * cntClmn);
			DrawText(hdc, mult.c_str(), -1, &loopRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	for (int cnt = 1; cnt < maxNumber + 1; ++cnt) {
		RECT leftHeader{ cnt * pnt.x,0,(cnt + 1) * pnt.x, pnt.y }, topHeader{ 0, cnt * pnt.y,pnt.x, (cnt + 1) * pnt.y };
		std::string txt = std::to_string(cnt);
		DrawText(hdc, txt.c_str(), -1, &topHeader, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		DrawText(hdc, txt.c_str(), -1, &leftHeader, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	SelectObject(hdc, oldFont);
	DeleteObject(currFont);
}

void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT: 
			CHOOSEFONT chf;
			chf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
			ChooseFont(&chf);
			CHOOSEFONT cf;
			ZeroMemory(&cf, sizeof cf);
			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
			cf.hwndOwner = *this;
			cf.lpLogFont = &logFont;
			cf.rgbColors = colorRef;
			ChooseFont(&cf);
			colorRef = cf.rgbColors;
			InvalidateRect(*this, 0, TRUE);
			break;
		case ID_NUMBER: {
			NumberDialog numDlg;
			numDlg.number = maxNumber;
			numDlg.DoModal(NULL, *this);
			maxNumber = numDlg.number;
			InvalidateRect(*this, 0, TRUE);
			break;
		}
		case ID_EXIT: 
			DestroyWindow(*this); 
			break;
	}
}

void MainWindow::OnDestroy(){
	::PostQuitMessage(0);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MainWindow wnd;	
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"),
		(int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.Run();
}
