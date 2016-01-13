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
	SetInt(IDC_EDIT1, n);
	return true;
}
bool NumberDialog::OnOK(){
	try {
		n = GetInt(IDC_EDIT1);
	}
	catch (XCtrl&) {
		MessageBox(*this, _T("Primam samo brojke!"), NULL, MB_OK);
		return false;
	}
	return true;
}


 
void MainWindow::OnPaint(HDC hdc){

	RECT rc; GetClientRect(*this, &rc);
	MoveToEx(hdc, 0, rc.bottom/maxNum, NULL);	LineTo(hdc, rc.right, rc.bottom/maxNum);//crtanje linije horizontala
	MoveToEx(hdc, rc.right/maxNum, 0, NULL);    LineTo(hdc, rc.right/maxNum, rc.bottom);//crtanje linije vertikala

	TCHAR s[10];
	for (int i = 0; i < maxNum+1; ++i) {//popunjavanje brojeva vertikala
		_stprintf(s, _T("%d"), i);
		RECT r = { 0, i*rc.bottom / maxNum, rc.right / maxNum, (i + 1)*rc.bottom / maxNum };
		DrawText(hdc, s, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}
	for (int i = 1; i < maxNum + 1; ++i) {//popunjavanje brojeva horizontala
		_stprintf(s, _T("%d"), i);
		RECT r = {  i*rc.right / maxNum, 0 ,(i + 1)*rc.right/maxNum,rc.bottom/maxNum };
		DrawText(hdc, s, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		for (int j = 1; j < maxNum + 1; ++j) {//množenje vertikla i horizontala
			_stprintf(s, _T("%d"), i*j);
			RECT r = {i*rc.right / maxNum,j*rc.bottom / maxNum,(i+1)*rc.right / maxNum,(j+1)*rc.bottom / maxNum };
			DrawText(hdc, s, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT: 
			if (GetFont(lf, col))
				InvalidateRect(*this, NULL, true);

			break;
		case ID_NUMBER: 
		{
			NumberDialog dlg;	dlg.n = maxNum;
			if (dlg.DoModal(0, *this) == IDOK) {
				maxNum = dlg.n;
				InvalidateRect(*this, NULL, true);
			}
		}

			break;
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
