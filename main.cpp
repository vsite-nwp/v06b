#include "main.h"
#include "rc.h"

int NumberDialog::IDD() {
	return IDD_NUMBER;
}
bool NumberDialog::OnInitDialog() {
	SetInt(IDC_EDIT1, newNumber);
	return true;
}
bool NumberDialog::OnOK() {
	try {
		newNumber = GetInt(IDC_EDIT1);
	}
	catch (XCtrl&) {
		return false;
	}
	return true;

}

MainWindow::MainWindow() : number(6) {
	lfont = { 0 };
	HDC hdc = GetDC(0);
	strcpy_s(lfont.lfFaceName, "Arial");
	lfont.lfHeight = -20 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	ReleaseDC(0, hdc);
}


void MainWindow::OnPaint(HDC hdc) {
	RECT box;
	GetClientRect(*this, &box);

	HFONT nFont = CreateFontIndirect(&lfont);
	HFONT cFont = (HFONT)SelectObject(hdc, nFont);

	int sqWidth = box.right / (number + 1);
	int sqHeight = box.bottom / (number + 1);

	MoveToEx(hdc, 0, sqHeight, NULL);
	LineTo(hdc, box.right, sqHeight);
	MoveToEx(hdc, sqWidth, 0, NULL);
	LineTo(hdc, sqWidth, box.bottom);

	const DWORD style = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	std::string broj;

	for (int i = 0; i <= number; i++) {

		for (int j = 0; j <= number; j++) {

			if (i == 0 && j == 0)
				continue;
			else if (i == 0)
				broj = std::to_string(j);
			else if (j == 0)
				broj = std::to_string(i);
			else
				broj = std::to_string(i * j);
			
			RECT paintBox = { i * sqWidth, j * sqHeight, (i + 1) * sqWidth , (j + 1)*sqHeight };
			DrawText(hdc, broj.c_str(), -1, &paintBox, style);
		}
	}

	SelectObject(hdc, cFont);
	DeleteObject(nFont);
}

void MainWindow::OnCommand(int id) {
	switch (id) {
	case ID_FONT:
	{
		CHOOSEFONT cf;
		ZeroMemory(&cf, sizeof cf);
		cf.lStructSize = sizeof cf;
		cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
		cf.lpLogFont = &lfont;
		cf.hwndOwner = *this;
		if (ChooseFont(&cf)) {
			InvalidateRect(*this, NULL, true);
		}
		break;
	}
	case ID_NUMBER:
	{
		NumberDialog nd;
		nd.newNumber = number;
		if (nd.DoModal(0, *this) == IDOK) {
			number = nd.newNumber;
			InvalidateRect(*this, NULL, true);
		}
		break;
	}
	case ID_EXIT:
		DestroyWindow(*this);
		break;
	}
}

void MainWindow::OnDestroy() {
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