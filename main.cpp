#include "main.h"
#include "rc.h"
#include <string>

int number_dialog::idd() const {
	return IDD_NUMBER; 
}
bool number_dialog::on_init_dialog(){
	set_int(IDC_EDIT1, userNum);
	return true;
}
bool number_dialog::on_ok(){
	try{
	userNum = get_int(IDC_EDIT1);
	} catch(std::runtime_error e) {
		MessageBox(*this, _T("Number must be inputed!"), _T("Error"), MB_OK|MB_ICONERROR);
	}

	return true;
}
 
void main_window::on_paint(HDC hdc){

	RECT rectWindow;
	GetClientRect(*this, &rectWindow);

	int width = rectWindow.right / (nums + 1), height = rectWindow.bottom / (nums + 1);
	HFONT myFont = CreateFontIndirect(&font);
	HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, myFont));

	MoveToEx(hdc, 0, height, 0);
	LineTo(hdc, rectWindow.right, height);
	MoveToEx(hdc, width, 0, 0);
	LineTo(hdc, width, rectWindow.bottom);
	SetTextColor(hdc, fontColor);
	for(int count = 1; count < nums + 1; ++count) {
		RECT left{0, height * count, width, (count+1) * height}, top {count * width, 0, (count+1) * width, height};
		std::wstring text = std::to_wstring(count);
		DrawText(hdc, text.c_str(), -1, &left, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		DrawText(hdc, text.c_str(), -1, &top, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}

	for(int i = 1; i <= nums ; ++i) {
		for(int j = 1;j <= nums ; ++j) {
			RECT rect{j * width, i * height, (j + 1) * width, (i + 1) * height};
			std::wstring num = std::to_wstring(i * j);
			DrawText(hdc, num.c_str(), -1, &rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
	}

	SelectObject(hdc, oldFont);
	DeleteObject(myFont);
}

main_window::main_window() {
	nums = 5;
	fontColor = RGB(0, 0, 0);
	ZeroMemory(&font, sizeof font);
	_tcscpy(font.lfFaceName, _T("Arial"));
	HDC value = GetDC(0);
	font.lfHeight = -10 * GetDeviceCaps(value, LOGPIXELSY) / 72;
	ReleaseDC(0, value);
}

void main_window::on_command(int id){
	switch(id){
		case ID_FONT: 
			CHOOSEFONT chooseFont;
			ZeroMemory(&chooseFont, sizeof chooseFont);
			chooseFont.lStructSize = sizeof chooseFont;
			chooseFont.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
			chooseFont.lpLogFont = &font;
			chooseFont.hwndOwner = *this;
			chooseFont.rgbColors = fontColor;
			if(ChooseFont(&chooseFont)){
				fontColor = chooseFont.rgbColors;
				InvalidateRect(*this, 0, TRUE);
			}

			break;
		case ID_NUMBER: {
			number_dialog numsDialog;
			numsDialog.userNum = nums;
			
			if (numsDialog.do_modal(NULL, *this) == IDOK) {
				nums = numsDialog.userNum;
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window wnd;	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.run();
}
