#include "main.h"
#include "rc.h"
#include <string>

int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, edNum);
	return true;
}
bool NumberDialog::OnOK(){
	edNum = GetInt(IDC_EDIT1);
	return true;
}

MainWindow::MainWindow() :numNumbers(10) {
	fontColor = RGB(0, 0, 0);
	ZeroMemory(&font, sizeof font);
	_tcscpy(font.lfFaceName, _T("Arial"));
	HDC hdc = GetDC(0);
	font.lfHeight = -10 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	ReleaseDC(0,hdc);
}
 
void MainWindow::OnPaint(HDC hdc){
	RECT rct; GetClientRect(*this, &rct);
	POINT pnt{ rct.right / (numNumbers+1), rct.bottom / (numNumbers+1) };

	HFONT currFont = CreateFontIndirect(&font);
	HFONT oldFont=(HFONT)SelectObject(hdc, currFont);

	//drawing lines for table edges
	MoveToEx(hdc, pnt.x, 0, 0);
	LineTo(hdc, pnt.x, rct.bottom);
	MoveToEx(hdc, 0, pnt.y, 0);
	LineTo(hdc, rct.right, pnt.y);
	SetTextColor(hdc, fontColor);
	
	//headers
	for (int cnt = 1; cnt < numNumbers + 1; ++cnt) {
	RECT leftHeader{ cnt*pnt.x,0,(cnt + 1)*pnt.x, pnt.y }, topHeader{ 0, cnt*pnt.y,pnt.x, (cnt + 1)*pnt.y };
	std::string txt = std::to_string(cnt);
	DrawText(hdc, txt.c_str(), -1, &topHeader, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DrawText(hdc, txt.c_str(), -1, &leftHeader, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	//table drawing
	//cntRow->cords for Y (Line we are positioned) ::::::::: cntClmn->cords for x (column we are positioned)
	for (int cntRow = 1; cntRow <= numNumbers+1; ++cntRow) {
		for (int cntClmn = 1; cntClmn <= numNumbers+1; ++cntClmn) {
			RECT loopRect{cntRow*pnt.x, cntClmn*pnt.y, (cntRow+1)*pnt.x, (cntClmn+1)*pnt.y};
			std::string mult = std::to_string(cntRow*cntClmn);
			DrawText(hdc, mult.c_str(), -1, &loopRect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
	}

	SelectObject(hdc, oldFont);
	DeleteObject(currFont);
}

void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT:
			CHOOSEFONT cf;
			ZeroMemory(&cf, sizeof cf);
			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT|CF_SCREENFONTS|CF_EFFECTS;
			cf.hwndOwner = *this;
			cf.lpLogFont = &font;
			cf.rgbColors = fontColor;
			ChooseFont(&cf);
			fontColor = cf.rgbColors;
			InvalidateRect(*this, 0, TRUE);
			break;
		case ID_NUMBER: {
			NumberDialog numDlg;
			numDlg.edNum = numNumbers;
			numDlg.DoModal(NULL, *this);
			numNumbers = numDlg.edNum;
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
