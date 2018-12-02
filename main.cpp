#include "main.h"
#include "rc.h"

int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, uneseni_broj);
	return true;
}
bool NumberDialog::OnOK(){
	try{
		uneseni_broj = GetInt(IDC_EDIT1);

		if (uneseni_broj < 0)
		{
			MessageBox(*this, _T("Uneseni broj je manji od 0."), NULL, MB_OK); return false;
		}

	}catch (XCtrl msg) {

		MessageBox(*this, _T("Uneseni znak nije broj."), NULL, MB_OK); return false; }

	return true;
}

void MainWindow::OnPaint(HDC hdc){
	RECT client_rect;
	GetClientRect(*this, &client_rect);

	HGDIOBJ old_font = SelectObject(hdc, CreateFontIndirect(&lf));

	float dx = (float)client_rect.right / (max_broj+1);
	float dy = (float)client_rect.bottom / (max_broj+1);

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
	
	DeleteObject(SelectObject(hdc,old_font));

}
	
void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT:
			
			LOGFONT Font = lf;
			CHOOSEFONT cfont;			
			ZeroMemory(&cfont, sizeof cfont);
			cfont.lStructSize = sizeof cfont;
			cfont.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
			cfont.lpLogFont = &Font;
			cfont.hwndOwner = *this;
			if (ChooseFont(&cfont))
			{
				lf = Font;
				InvalidateRect(*this, 0, true);
			}
			
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