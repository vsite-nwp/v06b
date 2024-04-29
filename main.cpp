#include "main.h"
#include "rc.h"

int number_dialog::idd() const {
	return IDD_NUMBER; 
}
bool number_dialog::on_init_dialog(){
	return true;
}
bool number_dialog::on_ok(){
	inputNum = get_int(IDC_EDIT1);
	return true;
}

int number_dialog::get_input_num() {
	return inputNum;
}
 
void main_window::on_paint(HDC hdc){
	HFONT hFont = CreateFontIndirect(&lf);
	SelObj selectedObject(hdc, hFont);

	RECT rect;  // Left, top, right, bottom.
	GetClientRect(*this, &rect);

	int width = rect.right / (tableNum + 1);
	int height = rect.bottom / (tableNum + 1);

	MoveToEx(hdc, 0, height, 0);
	LineTo(hdc, rect.right, height);
	MoveToEx(hdc, width, 0, 0);
	LineTo(hdc, width, rect.bottom);

	tstring number;

	//int n = 1; number = std::to_wstring(n); // zašto mora w ako je tstring?

	// DrawText: style DT_VCENTER does not work without DT_SINGLELINE.

	// First row.
	for (int i = 1; i <= tableNum; ++i) {
		number = std::to_wstring(i);
		RECT current = { width * i, 0, (width * i) + width, height };
		DrawText(hdc, number.c_str(), -1, &current, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	// Rest of the table.
	for (int i = 1; i <= tableNum; ++i) {
		number = std::to_wstring(i);
		RECT current = { 0, height * i, width, height * i + height };
		DrawText(hdc, number.c_str(), -1, &current, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		for (int j = 1; j <= tableNum; ++j) {
			number = ::std::to_wstring(i * j);
			RECT current = { width * j, height * i, (width * j) + width, (height * i) + height };
			DrawText(hdc, number.c_str(), -1, &current, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}
}

void main_window::on_command(int id){
	switch(id){
		case ID_FONT:
			wcscpy(lf.lfFaceName, _T("Comic Sans MS"));

			CHOOSEFONT cf;
			ZeroMemory(&cf, sizeof cf);
			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT
				| CF_SCREENFONTS | CF_EFFECTS;
			cf.lpLogFont = &lf;
			cf.hwndOwner = *this;

			if (ChooseFont(&cf)) {
				InvalidateRect(*this, 0, TRUE);
			}
			break;
		case ID_NUMBER: {
			number_dialog nd;
			nd.do_modal(0, *this);
			tableNum = nd.get_input_num();
			InvalidateRect(*this, 0, TRUE);
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

main_window::main_window() : lf { 0 }, tableNum { 6 } {
}

SelObj::SelObj(HDC hdc, HGDIOBJ hObj) : hdc(hdc), hOld(::SelectObject(hdc, hObj)) {}

SelObj::~SelObj() {::SelectObject(hdc, hOld); }

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window wnd;	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.run();
}
