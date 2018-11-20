#include "main.h"
#include "rc.h"

int NumberDialog::IDD() {
	return IDD_NUMBER;
}
bool NumberDialog::OnInitDialog() {
	SetInt(IDC_EDIT1, MaxNumber);
	return true;
}
bool NumberDialog::OnOK() {
	try {
		MaxNumber = GetInt(IDC_EDIT1);
	}
	catch (XCtrl) {
		return false;
	}
	return true;
}

void MainWindow::OnPaint(HDC hdc) {

	HGDIOBJ previousFont = SelectObject(hdc, CreateFontIndirect(&Font));

	RECT rect;
	GetClientRect(*this, &rect);
	float cellWidth = static_cast<float>(rect.right) / (MaxNumber + 1);
	float cellHeight = static_cast<float>(rect.bottom) / (MaxNumber + 1);
	std::basic_string<TCHAR> value;

	for (int i = 0; i <= MaxNumber; i++)
	{
		for (int j = 0; j <= MaxNumber; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			else if (i == 0)
			{
				value = std::to_string(j);
			}
			else if (j == 0)
			{
				value = std::to_string(i);
			}
			else {
				value = std::to_string(i * j);
			}

			RECT rect;
			rect.left = i * cellWidth;
			rect.right = i * cellWidth + cellWidth;
			rect.top = j * cellHeight;
			rect.bottom = j * cellHeight + cellHeight;

			DrawText(hdc, value.c_str(), value.length(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	MoveToEx(hdc, 0, cellHeight, NULL);
	LineTo(hdc, rect.right, cellHeight);

	MoveToEx(hdc, cellWidth, 0, NULL);
	LineTo(hdc, cellWidth, rect.bottom);

	DeleteObject(SelectObject(hdc, previousFont));
}

void MainWindow::OnCommand(int id) {
	switch (id) {
	case ID_FONT:
	{
		LOGFONT lf = Font;

		CHOOSEFONT cf;
		ZeroMemory(&cf, sizeof cf);
		cf.lStructSize = sizeof cf;
		cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
		cf.lpLogFont = &lf;
		if (ChooseFont(&cf)) {
			Font = lf;
			InvalidateRect(*this, NULL, true);
		}

	}
		
	break;

	case ID_NUMBER:
	{
		NumberDialog numberDialog;
		numberDialog.MaxNumber = MaxNumber;
		if (numberDialog.DoModal(NULL, *this) == IDOK)
		{
			MaxNumber = numberDialog.MaxNumber;
			InvalidateRect(*this, NULL, true);
		}

	}

	break;

	case ID_EXIT:
		DestroyWindow(*this);
		break;
	}
}

void MainWindow::OnDestroy() {
	::PostQuitMessage(0);
}

MainWindow::MainWindow() {
	Font = { 0 };
	HDC hdc = GetDC(0);
	strcpy(Font.lfFaceName, "Arial");
	Font.lfHeight = -12 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	ReleaseDC(0, hdc);
	MaxNumber = 10;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MainWindow wnd;
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"),
		(int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.Run();
}
