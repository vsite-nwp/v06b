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
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP", 
		(int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.Run();
}
