#include "main.h"
#include "rc.h"

int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, uneseni_broj+1);
	return true;
}
bool NumberDialog::OnOK(){
	try{

		uneseni_broj = GetInt(IDC_EDIT1);				
	}catch (XCtrl msg) {

		MessageBox(*this, _T("Uneseni znak nije broj."), NULL, MB_OK); return false; }

	return true;
}

bool make_font(LOGFONT &lf, COLORREF &color) {
	CHOOSEFONT cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.lStructSize = sizeof(cf);
	cf.rgbColors = color;
	cf.lpLogFont = &lf;
	cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	if (ChooseFont(&cf)) { color = cf.rgbColors; return true; }
	return false;
}

 
void MainWindow::OnPaint(HDC hdc){
	RECT client_rect;
	GetClientRect(*this, &client_rect);

	HFONT new_font = CreateFontIndirect(&lf);
	HGDIOBJ old_font = SelectObject(hdc, new_font);

	float dx = client_rect.right / (max_broj+1);
	float dy = client_rect.bottom / (max_broj+1);

	MoveToEx(hdc, 0, dy, NULL);
	LineTo(hdc, client_rect.right, dy);

	MoveToEx(hdc, dx, 0, NULL);
	LineTo(hdc, dx, client_rect.bottom);

	TCHAR num[15];
	// okomiti
	for (int count = 1; count <= max_broj; ++count) { 
		_stprintf(num, _T("%d"), count);
		RECT rect = { 0, count * dy, dx, (count + 1) * dy };
		DrawText(hdc, num, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	// vodoravni
	for (int count = 1; count <= max_broj; ++count) {
		_stprintf(num, _T("%d"), count);
		RECT rect = { count * dx, 0, (count + 1) * dx, dy };
		DrawText(hdc, num, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		//mnozenje
		for (int mp = 1; mp <= max_broj; ++mp) {
			_stprintf(num, _T("%d"), count*mp);
			RECT rect = { count * dx, mp * dy, (count + 1) * dx, (mp + 1) * dy };
			DrawText(hdc, num, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	SelectObject(hdc, old_font);
	DeleteObject(new_font);

}
	
void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT: 
			if (make_font(lf, color)) InvalidateRect(*this, NULL, true);
			break;
		case ID_NUMBER:
		{
			NumberDialog dlg;
			dlg.uneseni_broj = max_broj;
			if (dlg.DoModal(0, *this) == IDOK){
				max_broj = dlg.uneseni_broj;
				InvalidateRect(*this, NULL, true);
			}
		}
			InvalidateRect(*this, NULL, TRUE);
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