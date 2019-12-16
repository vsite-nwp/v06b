#include "main.h"
#include "rc.h"



int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, newMax);
	return true;
}
bool NumberDialog::OnOK(){
	try {
		newMax = GetInt(IDC_EDIT1);
	}
	catch (XCtrl&) {
		return false;
	}
	return true;

}

MainWindow::MainWindow() : maxNumber(12) {
	lfont = { 0 };
	HDC hdc = GetDC(0);
	strcpy(lfont.lfFaceName, "Arial");
	lfont.lfHeight = -20 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	ReleaseDC(0, hdc);
}
 
void MainWindow::OnPaint(HDC hdc){
	RECT rct;
	GetClientRect(*this, &rct);

	HFONT nFont = CreateFontIndirect(&lfont);
	HFONT cFont = (HFONT)SelectObject(hdc, nFont);

	int sqWidth = rct.right / (maxNumber+1);
	int sqHeight = rct.bottom / (maxNumber+1);

	MoveToEx(hdc, 0, sqHeight, NULL);
	LineTo(hdc, rct.right, sqHeight);
	MoveToEx(hdc, sqWidth, 0, NULL);
	LineTo(hdc, sqWidth, rct.bottom);

	const DWORD style = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	TCHAR num[16];

	for (int i = 0; i <= maxNumber; i++) {

		for (int j = 0; j <= maxNumber; j++) {

			if (i == 0 && j == 0)
				continue;
			else if (j == 0)
				_stprintf(num, _T("%d"), i);
			else if (i == 0)
				_stprintf(num, _T("%d"), j);
			else
				_stprintf(num, _T("%d"), (i*j));

			RECT rctO = { i * sqWidth, j * sqHeight, (i + 1) * sqWidth , (j + 1)*sqHeight };
			DrawText(hdc, num, -1, &rctO, style);
		}
	}
		
	SelectObject(hdc, cFont);
	DeleteObject(nFont);
}

void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT: 
		{
			CHOOSEFONT cf;
			ZeroMemory(&cf, sizeof cf);
			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
			cf.lpLogFont = &lfont;
			if (ChooseFont(&cf)) {
				InvalidateRect(*this, NULL, true);
			}
			break;
		}
		case ID_NUMBER: 
		{
			NumberDialog nd;
			nd.newMax = maxNumber;
			if (nd.DoModal(0, *this) == IDOK) {
				maxNumber = nd.newMax;
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
