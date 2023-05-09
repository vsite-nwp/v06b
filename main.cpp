#include "main.h"
#include "rc.h"
#include <string>

int number_dialog::idd() const {
	return IDD_NUMBER; 
}

bool number_dialog::on_init_dialog(){
	set_int(IDC_EDIT1, UserNumber);
	return true;
}

bool number_dialog::on_ok(){
	try {
		UserNumber = get_int(IDC_EDIT1);
	}
	catch (std::runtime_error e) {
		MessageBox(*this, _T("You forgot to write number"), _T("Error"), MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void main_window::on_paint(HDC hdc){
	RECT rectWindow;
	GetClientRect(*this, &rectWindow);

	int width = rectWindow.right / (numbers + 1);
	int height = rectWindow.bottom / (numbers + 1);

	HFONT newFont = CreateFontIndirect(&font);
	HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, newFont));

	MoveToEx(hdc, 0, height, 0);
	LineTo(hdc, rectWindow.right, height);
	MoveToEx(hdc, width, 0, 0);
	LineTo(hdc, width, rectWindow.bottom);
	SetTextColor(hdc, fc);

	for (int i = 1; i < numbers + 1; ++i) {
		RECT left{ 0, height * i, width, (i + 1) * height }, top{ i * width, 0, (i + 1) * width, height };
		std::wstring text = std::to_wstring(i);
		DrawText(hdc, text.c_str(), -1, &left, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		DrawText(hdc, text.c_str(), -1, &top, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	for (int i = 1; i <= numbers; ++i) {
		for (int j = 1; j <= numbers; ++j) {
			RECT rect{ j * width, i * height, (j + 1) * width, (i + 1) * height };
			std::wstring num = std::to_wstring(i * j);
			DrawText(hdc, num.c_str(), -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	SelectObject(hdc, oldFont);
	DeleteObject(newFont);

}

void main_window::on_command(int id){
	switch(id){
		case ID_FONT:
			CHOOSEFONT cf;
			ZeroMemory(&cf, sizeof cf);
			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
			cf.lpLogFont = &font;
			cf.hwndOwner = *this;
			cf.rgbColors = fc;
			
			if (ChooseFont(&cf)) {
				fc = cf.rgbColors;
				InvalidateRect(*this, 0, TRUE);
			}

			break;
		case ID_NUMBER: {
			number_dialog numberDialog;
			numberDialog.UserNumber = numbers;

			if (numberDialog.do_modal(NULL, *this) == IDOK) {
				numbers = numberDialog.UserNumber;
				InvalidateRect(*this, 0, TRUE);
			}
		}
			break;

		case ID_EXIT:
			::DestroyWindow(*this);
			break;
	}
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

main_window::main_window() {
	numbers = 10;
	fc = RGB(0, 0, 0);
	ZeroMemory(&font, sizeof font);
	_tcscpy(font.lfFaceName, _T("Arial"));
	HDC value = GetDC(0);
	font.lfHeight = -10 * GetDeviceCaps(value, LOGPIXELSY) / 72;
	ReleaseDC(0, value);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window wnd;	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.run();
}
