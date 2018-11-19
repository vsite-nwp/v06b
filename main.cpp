#include "main.h"
#include "rc.h"

int NumberDialog::IDD() {
	return IDD_NUMBER;
}
bool NumberDialog::OnInitDialog() {
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

	if (Font == NULL || MaxNumber == NULL)
	{
		return;
	}

	HGDIOBJ selectedFont = SelectObject(hdc, Font);
	int cellWidth = FontSize / 4;
	std::basic_string<TCHAR> value;

	for (int i = 0; i <= MaxNumber; i++)
	{
		for (int j = 0; j <= MaxNumber; j++)
		{
			if (i == 0 && j == 0)
			{
				value = "";
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
			rect.top = j * cellWidth;
			rect.bottom = j * cellWidth + cellWidth;

			DrawText(hdc, value.c_str(), value.length(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	MoveToEx(hdc, 0, cellWidth, NULL);
	LineTo(hdc, MaxNumber * cellWidth + cellWidth, cellWidth);

	MoveToEx(hdc, cellWidth, 0, NULL);
	LineTo(hdc, cellWidth, MaxNumber * cellWidth + cellWidth);
}

void MainWindow::OnCommand(int id) {
	switch (id) {
	case ID_FONT:
	{
		LOGFONT lf = { 0 };
		CHOOSEFONT cf;
		ZeroMemory(&cf, sizeof cf);
		cf.lStructSize = sizeof cf;
		cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
		cf.lpLogFont = &lf;
		if (ChooseFont(&cf)) {
			FontSize = cf.iPointSize;
			Font = CreateFontIndirect(&lf);
		}
	}

	if (Font != NULL && MaxNumber != NULL)
	{
		RECT rect;
		GetClientRect(GetActiveWindow(), &rect);
		InvalidateRect(GetActiveWindow(), &rect, true);
	}

	break;

	case ID_NUMBER:
	{
		NumberDialog numberDialog;
		if (numberDialog.DoModal(NULL, GetActiveWindow()) == IDOK)
		{
			MaxNumber = numberDialog.MaxNumber;
		}
	}

	if (Font != NULL && MaxNumber != NULL)
	{
		RECT rect;
		GetClientRect(GetActiveWindow(), &rect);
		InvalidateRect(GetActiveWindow(), &rect, true);
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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MainWindow wnd;
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"),
		(int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.Run();
}
