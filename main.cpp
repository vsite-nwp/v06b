#include "main.h"
#include "rc.h"

int number_dialog::idd() const {
	return IDD_NUMBER;
}
bool number_dialog::on_init_dialog() {
	set_int(IDC_EDIT1, new_max_broj);
	return true;
}
bool number_dialog::on_ok() {
	try {
		new_max_broj = get_int(IDC_EDIT1);
	}
	catch (std::runtime_error) {
		return false;
	}
	return true;

}

void main_window::on_paint(HDC hdc) {
	RECT rect;
	GetClientRect(*this, &rect);
	HFONT nFont = CreateFontIndirect(&lf);
	HFONT cFont = (HFONT)SelectObject(hdc, nFont);
	int width = rect.right / (max_broj + 1);
	int height = rect.bottom / (max_broj + 1);

	MoveToEx(hdc, 0, height, NULL);
	LineTo(hdc, rect.right, height);
	MoveToEx(hdc, width, 0, NULL);
	LineTo(hdc, width, rect.bottom);

	const DWORD style = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	std::wstring broj;

	for (int i = 0; i <= max_broj; i++) {

		for (int j = 0; j <= max_broj; j++) {

			if (i == 0 || j == 0) {
				broj = std::to_wstring(i + j);
			}
			else {
				broj = std::to_wstring(i * j);
			}
			RECT rectO = { i * width, j * height, (i + 1) * width, (j + 1) * height };
			DrawText(hdc, broj.c_str(), -1, &rectO, style);
		}
	}

	SelectObject(hdc, cFont);
	DeleteObject(nFont);
}

void main_window::on_command(int id) {
	switch (id) {
	case ID_FONT:
		LOGFONT temp = lf;
		CHOOSEFONT cf;
		ZeroMemory(&cf, sizeof cf);
		cf.lStructSize = sizeof cf;
		cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
		cf.hwndOwner = *this;
		cf.lpLogFont = &temp;
		if (ChooseFont(&cf)) {
			lf = temp;
			InvalidateRect(*this, NULL, true);
		}
		break;
	case ID_NUMBER: {
		number_dialog nd;
		nd.new_max_broj = max_broj;
		if (nd.do_modal(0, *this) == IDOK) {
			max_broj = nd.new_max_broj;
			InvalidateRect(*this, NULL, true);
		}
		break;
	}
	case ID_EXIT:
		::DestroyWindow(*this);
		break;
	}
}

void main_window::on_destroy() {
	::PostQuitMessage(0);
}

main_window::main_window() : max_broj(16)
{
	lf = { 0 };
	HDC hdc = GetDC(0);
	wcscpy(lf.lfFaceName, _T("Arial"));
	lf.lfHeight = -MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(0, hdc);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
