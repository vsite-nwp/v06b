#include "main.h"
#include "rc.h"

int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1,num);
	return true;
}
bool NumberDialog::OnOK(){
	try {
		num=GetInt(IDC_EDIT1)+1;
	}
	catch (XCtrl&) {
		return false;
	}
	return true;
}

void MainWindow::OnPaint(HDC hdc){
	RECT rect;
	GetClientRect(*this, &rect);
	
	HFONT hf = CreateFontIndirect(&lf);
	SelectObject(hdc, hf);

	MoveToEx(hdc, 0, rect.bottom/MaxNum, NULL);
	LineTo(hdc, rect.right, rect.bottom / MaxNum);
	MoveToEx(hdc, rect.right / MaxNum, 0, NULL);
	LineTo(hdc, rect.right / MaxNum, rect.bottom);
	
	const DWORD style = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	TCHAR buf[15];
	for (int i = 1; i <= MaxNum; i++) {
		RECT rc = {0, i*rect.bottom/MaxNum, rect.right/MaxNum, (i+1)*rect.bottom/MaxNum};
		_stprintf(buf, TEXT("%d"), i);
		DrawText(hdc, buf, -1, &rc,style);
	}

	for (int i = 1; i <= MaxNum; i++) {
		RECT rc = {  (i+1)*rect.right / MaxNum,0, i*rect.right / MaxNum, rect.bottom / MaxNum };
		_stprintf(buf, TEXT("%d"), i);
		DrawText(hdc, buf, -1, &rc, style);
		for (int j = 1; j <= MaxNum; j++) {
			RECT rc = { i*rect.right/MaxNum, j*rect.bottom / MaxNum, (i+1)*rect.right / MaxNum, (j+1)*rect.bottom / MaxNum };
			_stprintf(buf, TEXT("%d"), i*j);
			DrawText(hdc, buf, -1, &rc, style);
		}
	}
	
}

void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT: 
			CHOOSEFONT font;
			ZeroMemory(&font, sizeof font);
			font.lStructSize = sizeof font;
			font.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
			font.lpLogFont = &lf;
			font.hwndOwner = *this;
			ChooseFont(&font);
			break;
		case ID_NUMBER: 
		{
			NumberDialog ND;
			ND.num=10;
			if (ND.DoModal(0, *this) == IDOK) {
				MaxNum = ND.num;
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