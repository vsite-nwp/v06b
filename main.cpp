#include "main.h"
#include "rc.h"

bool GetFont(LOGFONT& lf, COLORREF& col) {
	CHOOSEFONT cf;
	ZeroMemory(&cf, sizeof cf);
	cf.lStructSize = sizeof cf;
	cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
	cf.lpLogFont = &lf;
	cf.rgbColors = col;
	if (ChooseFont(&cf)) {
		col = cf.rgbColors;
		return true;
	}
	return false;
}

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

 
void MainWindow::OnPaint(HDC hdc){
	RECT rc; GetClientRect(*this, &rc);
	MoveToEx(hdc, 0, rc.bottom / maxNumber, NULL);	
	LineTo(hdc, rc.right, rc.bottom / maxNumber);

	MoveToEx(hdc, rc.right / maxNumber, 0, NULL);   
	LineTo(hdc, rc.right / maxNumber, rc.bottom);

	TCHAR s[10];
	for (int i = 0; i < maxNumber + 1; ++i) {
		_stprintf(s, _T("%d"), i);
		RECT r = { 0, i * rc.bottom / maxNumber, rc.right / maxNumber, (i + 1) * rc.bottom / maxNumber };
		DrawText(hdc, s, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	for (int i = 1; i < maxNumber + 1; ++i) {
		_stprintf(s, _T("%d"), i);
		RECT r = { i * rc.right / maxNumber, 0 ,(i + 1) * rc.right / maxNumber,rc.bottom / maxNumber };
		DrawText(hdc, s, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		for (int j = 1; j < maxNumber + 1; ++j) {
			_stprintf(s, _T("%d"), i * j);
			RECT r = { i * rc.right / maxNumber,j * rc.bottom / maxNumber,(i + 1) * rc.right / maxNumber,(j + 1) * rc.bottom / maxNumber };
			DrawText(hdc, s, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT: 
			if (GetFont(logFont, colorRef))
				InvalidateRect(*this, NULL, true);
			break;
		case ID_NUMBER: {
			NumberDialog dlg;	dlg.number = maxNumber;
			if (dlg.DoModal(0, *this) == IDOK) {
				maxNumber = dlg.number;
				InvalidateRect(*this, NULL, true);
			}
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
