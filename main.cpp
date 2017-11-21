#include "main.h"
#include "rc.h"

int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, requestNum );
	return true;
}
bool NumberDialog::OnOK(){
	try { 
		requestNum = GetInt(IDC_EDIT1); 
	}
	catch (XCtrl ) {
		 return false;
	}
	return true;
}

void MainWindow::OnPaint(HDC hdc){
	RECT rc;
	GetClientRect(*this, &rc);
	int x = rc.right/(maxNum+1);
	int y = rc.bottom / (maxNum+1);

	HFONT font = CreateFontIndirect(&lf);
	HFONT oldFont =(HFONT) SelectObject(hdc, font);
	SetTextColor(hdc, color);

	MoveToEx(hdc, 0, y, NULL);
	LineTo(hdc, rc.right, y);
	MoveToEx(hdc, x, 0, NULL);
	LineTo(hdc, x, rc.bottom);

	for (int i = 1; i <= maxNum; i++) {
		std::string s = std::to_string(i);
		RECT vodoravno = { i*x,0,(i + 1)*x,y };
		RECT okomito = { 0,i*y,x,(i + 1)*y };
		DrawText(hdc, s.c_str(), -1, &vodoravno, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		DrawText(hdc, s.c_str(), -1, &okomito, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		for (int j = 1; j <= maxNum; j++) {
			s = std::to_string(i*j);
			RECT kucica = {j*x,i*y,(j+1)*x,(i+1)*y};
			DrawText(hdc, s.c_str(), -1, &kucica, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}	
	}

	SelectObject(hdc, oldFont);
	DeleteObject(font);
}

void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT: 
			CHOOSEFONT cf;
			ZeroMemory(&cf, sizeof cf);
			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT
				| CF_SCREENFONTS | CF_EFFECTS;
			cf.hwndOwner = *this;
			cf.lpLogFont = &lf;
			cf.rgbColors = color;
			if (ChooseFont(&cf)) {
				color = cf.rgbColors;
				InvalidateRect(*this, NULL, true);
			}
			break;
		case ID_NUMBER: {
			NumberDialog nD;
			nD.requestNum = maxNum;
			if (nD.DoModal(0, *this) == IDOK) {
				maxNum = nD.requestNum;
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
