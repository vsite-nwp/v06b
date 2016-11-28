#include "main.h"
#include "rc.h"

int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, req_number);
	return true;
}
bool NumberDialog::OnOK(){
	try { req_number = GetInt(IDC_EDIT1); }
	catch (XCtrl msg) {
		MessageBox(*this, _T("Uneseni znak nije broj."), NULL, MB_OK); return false; }
	return true;
}

 
void MainWindow::OnPaint(HDC hdc){
	RECT client_rect;
	GetClientRect(*this, &client_rect);
	
	MoveToEx(hdc, 0, client_rect.bottom / max_number, NULL);
	LineTo(hdc, client_rect.right, client_rect.bottom / max_number);

	MoveToEx(hdc, client_rect.right / max_number, 0, NULL);
	LineTo(hdc, client_rect.right / max_number, client_rect.bottom);

	TCHAR num[15];
	for (int count = 1; count <= max_number; ++count) { // okomiti brojevi
		_stprintf(num, _T("%d"), count);
		RECT rect = { 0, count * client_rect.bottom / max_number, client_rect.right / max_number, (count + 1) * client_rect.bottom / max_number };
		DrawText(hdc, num, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	for (int count = 1; count <= max_number; ++count) { // vodoravni brojevi
		_stprintf(num, _T("%d"), count);
		RECT rect = { count * client_rect.right / max_number, 0, (count + 1) * client_rect.right / max_number, client_rect.bottom / max_number };
		DrawText(hdc, num, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		for (int mp = 1; mp <= max_number; ++mp) { //mnozenje
			_stprintf(num, _T("%d"), count*mp);
			RECT rect = { count * client_rect.right / max_number, mp * client_rect.bottom / max_number, (count + 1) * client_rect.right / max_number, (mp + 1) * client_rect.bottom / max_number };
			DrawText(hdc, num, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
	
void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT: 
			if (make_font(lf, color)) InvalidateRect(*this, NULL, true);
			break;
		case ID_NUMBER:
		{
			NumberDialog dlg;
			dlg.req_number = max_number;
			if (dlg.DoModal(0, *this) == IDOK) { max_number = dlg.req_number; InvalidateRect(*this, NULL, true); }
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
