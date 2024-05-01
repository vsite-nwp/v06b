#include "main.h"
#include "rc.h"

int number_dialog::idd() const {
	return IDD_NUMBER; 
}
bool number_dialog::on_init_dialog(){
	set_int(IDC_EDIT1, inputNum);
	return true;
}
bool number_dialog::on_ok(){
	try {
		inputNum = get_int(IDC_EDIT1);
	}
	catch (std::runtime_error e) {
		tstring errorMessage = tstring(e.what(), e.what() + strlen(e.what())); // Constructor takes beginning and end of elements to copy.
		MessageBox(*this, errorMessage.c_str(), 0, MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

int number_dialog::get_input_num() {
	return inputNum;
}

void number_dialog::set_input_num(int number) {
	inputNum = number;
}
 
void main_window::on_paint(HDC hdc){
	HFONT hFont = CreateFontIndirect(&lf);
	SelObj selectedObject(hdc, hFont);
	SetTextColor(hdc, tableColor);

	RECT rect;  // Left, top, right, bottom.
	GetClientRect(*this, &rect);

	int width = rect.right / (tableNum + 1);
	int height = rect.bottom / (tableNum + 1);

	MoveToEx(hdc, 0, height, 0);
	LineTo(hdc, rect.right, height);
	MoveToEx(hdc, width, 0, 0);
	LineTo(hdc, width, rect.bottom);

	tstring number;

	// DrawText: style DT_VCENTER does not work without DT_SINGLELINE.
	
	// First row.
	for (int i = 1; i <= tableNum; ++i) {
		number = std::to_tstring(i);
		RECT current = { width * i, 0, (width * i) + width, height };
		DrawText(hdc, number.c_str(), -1, &current, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	// Rest of the table.
	for (int i = 1; i <= tableNum; ++i) {
		number = std::to_tstring(i);
		RECT current = { 0, height * i, width, height * i + height };
		DrawText(hdc, number.c_str(), -1, &current, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		for (int j = 1; j <= tableNum; ++j) {
			number = std::to_tstring(i * j);
			RECT current = { width * j, height * i, (width * j) + width, (height * i) + height };
			DrawText(hdc, number.c_str(), -1, &current, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	DeleteObject(hFont);
}

void main_window::on_command(int id){
	switch(id){
		case ID_FONT:
			CHOOSEFONT cf;
			ZeroMemory(&cf, sizeof cf);
			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT
				| CF_SCREENFONTS | CF_EFFECTS;
			cf.lpLogFont = &lf;
			cf.hwndOwner = *this;
			cf.rgbColors = tableColor;  // Set color to value from main_window constructor.	
			if (ChooseFont(&cf)) {  // If clicked on ok, returns true, CHOOSEFONT filled.
				tableColor = cf.rgbColors;  // Colors are not stored in LOGFONT!
				InvalidateRect(*this, 0, TRUE);
			}
			break;
		case ID_NUMBER: {
			number_dialog nd;
			nd.set_input_num(tableNum);  // Set number_dialog inputNum to main_window tableNum.
			if ((nd.do_modal(0, *this) == IDOK)) {  // 'do_modal' returns DialogBoxParam which returns ID of pressed control.
				tableNum = nd.get_input_num();
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

main_window::main_window() : lf{ 0 }, tableNum{ 6 }, tableColor{ RGB(0, 0, 0) } {
	_tcscpy(lf.lfFaceName, _T("Comic Sans MS"));
	HDC hdc = GetDC(0);
	lf.lfHeight = -13 * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	ReleaseDC(*this, hdc);
}

SelObj::SelObj(HDC hdc, HGDIOBJ hObj) : hdc(hdc), hOld(::SelectObject(hdc, hObj)) {}

SelObj::~SelObj() {::SelectObject(hdc, hOld); }

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window wnd;	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)), CW_USEDEFAULT, CW_USEDEFAULT, 500, 500);	
	return app.run();
}
