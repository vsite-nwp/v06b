#include "main.h"
#include "rc.h"


bool choose_font(LOGFONT& log_font_ref, COLORREF& color_ref) {
	//Prepare font specs for LOGFONT 
	CHOOSEFONT font_specs;
	ZeroMemory(&log_font_ref, sizeof(log_font_ref));
	ZeroMemory(&font_specs, sizeof(font_specs));
	font_specs.lStructSize = sizeof(font_specs);
	font_specs.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
	font_specs.lpLogFont = &log_font_ref;
	font_specs.rgbColors = color_ref;

	if (ChooseFont(&font_specs)) return true;
	return false;
}

int number_dialog::idd() const {
	return IDD_NUMBER; 
}

bool number_dialog::on_init_dialog() {
	dialog::set_int(IDC_EDIT1, this->num_to_multiply_to);
	return true;
}

bool number_dialog::on_ok(){
	try {
		num_to_multiply_to = dialog::get_int(IDC_EDIT1);
	}
	catch (std::runtime_error) {
		MessageBox(*this, _T("Error while entering number!"), 0, MB_OK);
		return false;
	}
	return true;
}
 
void main_window::on_paint(HDC hdc){
	
	font.lfHeight = -12 * (GetDeviceCaps(hdc, LOGPIXELSY) / 72); 
	int limit = this->limit_num + 1;

	RECT window_size;
	GetClientRect(*this, &window_size);

	//Draws vertical line of table
	MoveToEx(hdc, 0, window_size.bottom / limit, NULL);
	LineTo(hdc, window_size.right, window_size.bottom / limit);

	//Draws horizontal line of table
	MoveToEx(hdc, window_size.right / limit, 0, NULL);
	LineTo(hdc, window_size.right / limit, window_size.bottom);


	HFONT font_handle = CreateFontIndirect(&font);
	UINT font_styles = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	HGDIOBJ font_gdiObject = SelectObject(hdc, font_handle);
	SetTextColor(hdc, font_color);

	
	TCHAR num_char[8];

	//Draws numbers to multiply to left and top side of multiplication table
	for (int j = 1; j < limit; ++j) {

		swprintf_s(num_char, _T("%d"), j);

		//Move rectangle in which number is to be written in
		RECT vertical_num_rect = { 0, j * window_size.bottom / limit, window_size.right / limit, (j + 1) * window_size.bottom / limit };
		DrawText(hdc, num_char, -1, &vertical_num_rect, font_styles);

		RECT horizontal_num_rect = { j * (window_size.right / limit), 0, (j + 1) * (window_size.right / limit), (window_size.bottom / limit) };
		DrawText(hdc, num_char, -1, &horizontal_num_rect, font_styles);
	}

	//Draws and calculates multiplied numbers
	for (int j = 1; j < limit; ++j) {
		for (int k = 1; k < limit; ++k) {

			swprintf_s(num_char, _T("%d"), j * k);

			//Move rectangle in which number is to be written in
			RECT multiplied_nums_rect = { j * (window_size.right / limit), k * (window_size.bottom / limit),
			(j + 1) * (window_size.right / limit), (k + 1) * (window_size.bottom / limit) };
			DrawText(hdc, num_char, -1, &multiplied_nums_rect, font_styles);
		}
	}

	DeleteObject(SelectObject(hdc, font_handle));
}

void main_window::on_command(int id){
	number_dialog numDlg;
	switch(id){
		case ID_FONT: 
			if (choose_font(font, font_color)) {
				InvalidateRect(*this, 0, true);
			}
			break;
		case ID_NUMBER: 
			if (numDlg.do_modal(0, *this) == IDOK) {
				limit_num = numDlg.num_to_multiply_to;
				InvalidateRect(*this, 0, true);
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
