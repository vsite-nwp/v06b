#include "main.h"
#include "rc.h"

int number_dialog::idd() const {
	return IDD_NUMBER; 
}
bool number_dialog::on_init_dialog(){
	set_int(IDC_EDIT1, num);
	return true;
}
bool number_dialog::on_ok(){
	try {
		num = get_int(IDC_EDIT1);
	}
	catch (std::runtime_error){
		return false;
	}
	return true;
}
 
void main_window::on_paint(HDC hdc){
	RECT rect;
	GetClientRect(*this, &rect);

	HFONT newFont = CreateFontIndirect(&logfont);
	HFONT prevFont = (HFONT)SelectObject(hdc, newFont);

	int width = rect.right / (new_num_input + 1);
	int height = rect.bottom / (new_num_input + 1);

	HPEN hpen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	HGDIOBJ holdpen = SelectObject(hdc, hpen);
	MoveToEx(hdc, 0, height, NULL);
	LineTo(hdc, rect.right, height);
	MoveToEx(hdc, width, 0, NULL);
	LineTo(hdc, width, rect.bottom);
	DeleteObject(SelectObject(hdc, holdpen));


	const DWORD style = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	std::string displayNum;

	for (int i = 0; i <= new_num_input; i++) {
		if (i > 0) {
			displayNum = std::to_string(i);
		}
			

		for (int j = 0; j <= new_num_input; j++) {
			if (i == 0) {
				if (j < 1)
					displayNum = "X";
				else
					displayNum = std::to_string(j);
			}
			if (i > 0 && j > 0)
				displayNum = std::to_string(i * j);

			RECT numCell = { i * width, j * height, (i + 1) * width, (j + 1) * height };
			std::wstring displayStr = std::wstring(displayNum.begin(), displayNum.end());
			LPCWSTR sw = displayStr.c_str();
			DrawText(hdc, sw, -1, &numCell, style);
		}

	}
	SelectObject(hdc, prevFont);
	DeleteObject(newFont);
}

void main_window::on_command(int id){
	switch (id) {
	case ID_FONT:
		CHOOSEFONT cf;
		cf = {sizeof(CHOOSEFONT)};
		LOGFONT chosenFont = logfont;
		cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
		cf.hwndOwner = *this;
		cf.lpLogFont = &chosenFont;
		if (ChooseFont(&cf)) {
			logfont = chosenFont;
			InvalidateRect(*this, NULL, true);
		}
		break;
	case ID_NUMBER: {
		number_dialog nd;
		nd.num = new_num_input;
		if (nd.do_modal(0, *this) == IDOK) {
			new_num_input = nd.num;
			InvalidateRect(*this, NULL, true);
		}
		break;
	}
	case ID_EXIT: 
		::DestroyWindow(*this); 
		break;
	}
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

main_window::main_window() : new_num_input()
{
	logfont = { 0 };
	HDC hdc = GetDC(0);
	wcscpy(logfont.lfFaceName, _T("Garamond"));
	logfont.lfHeight = -10 * GetDeviceCaps(hdc, LOGPIXELSY) / 42;
	ReleaseDC(0, hdc);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window wnd;	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.run();
}
