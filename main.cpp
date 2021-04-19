#include "main.h"
#include "rc.h"
#include <list>

class multiplication {
	inline static int numToMultiplyTo = 0;
	inline static LOGFONT font;
	inline static COLORREF font_color = RGB(0, 0, 0);
	inline static CHOOSEFONT font_specs;
public:
	static void set_num(int number) { numToMultiplyTo = number; }
	static int get_num() { return numToMultiplyTo; }

	//Prepares font_specs for font dialog box
	static bool choose_font() {
		ZeroMemory(&font, sizeof(font));
		ZeroMemory(&font_specs, sizeof(font_specs));
		font_specs.lStructSize = sizeof(font_specs);
		font_specs.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
		font_specs.lpLogFont = &font;
		font_specs.rgbColors = font_color;

		if (ChooseFont(&font_specs)) return true;
		return false;
	}

	static LOGFONT get_font() { return font; }
	static COLORREF get_fontColor() { return font_color; }
};

void draw_lines(HDC context, HWND wnd_handle) {
	RECT window_size;
	GetClientRect(wnd_handle, &window_size);
	int num_to_multiply_to = multiplication::get_num() + 1;

	//Draws vertical line of table
	MoveToEx(context, 0, window_size.bottom / num_to_multiply_to, NULL);
	LineTo(context, window_size.right, window_size.bottom / num_to_multiply_to);

	//Draws horizontal line of table
	MoveToEx(context, window_size.right / num_to_multiply_to, 0, NULL);
	LineTo(context, window_size.right / num_to_multiply_to, window_size.bottom);
}

int number_dialog::idd() const {
	return IDD_NUMBER; 
}

bool number_dialog::on_init_dialog() {
	return true;
}

bool number_dialog::on_ok(){
	try {
		multiplication::set_num(dialog::get_int(IDC_EDIT1));
	}
	catch (std::runtime_error) {
		MessageBox(*this, _T("Error while entering number!"), 0, MB_OK);
		return false;
	}
	return true;
}
 
void main_window::on_paint(HDC hdc){
	draw_lines(hdc, *this);
	HFONT font_handle = CreateFontIndirect(&multiplication::get_font());
	UINT font_styles = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	HGDIOBJ font_gdiObject = SelectObject(hdc, font_handle);
	SetTextColor(hdc, multiplication::get_fontColor());

	RECT window_size; 
	GetClientRect(*this, &window_size);
	
	TCHAR num_char[8];
	int num_to_multiply_to = multiplication::get_num() + 1;

	//Draws numbers to multiply to left and top side of multiplication table
	for (int j = 1; j < num_to_multiply_to; ++j) {

		//Write num as unicode char string to num_char
		swprintf_s(num_char, _T("%d"), j); 

		//Move rectangle in which number is to be written in
		RECT vertical_num_rect = { 0, j * window_size.bottom / num_to_multiply_to, window_size.right / num_to_multiply_to, (j + 1) * window_size.bottom / num_to_multiply_to };
		DrawText(hdc, num_char, -1, &vertical_num_rect, font_styles);

		swprintf_s(num_char, _T("%d"), j);
		RECT horizontal_num_rect = { j * (window_size.right / num_to_multiply_to), 0, (j + 1) * (window_size.right / num_to_multiply_to), (window_size.bottom / num_to_multiply_to) };
		DrawText(hdc, num_char, -1, &horizontal_num_rect, font_styles);
	}

	//Draws and calculates multiplied numbers
	for (int j = 1; j < num_to_multiply_to; ++j) {
		for (int k = 1; k < num_to_multiply_to; ++k) {
			//Calculated current multiplied number
			int multiplied_num = j * k;

			//Write multiplied num as char string to num_char
			swprintf_s(num_char, _T("%d"), multiplied_num);
			
			//Move rectangle in which number is to be written in
			RECT multiplied_nums_rect = { j * (window_size.right / num_to_multiply_to), k * (window_size.bottom / num_to_multiply_to),
			(j + 1) * (window_size.right / num_to_multiply_to), (k + 1) * (window_size.bottom / num_to_multiply_to) };
			DrawText(hdc, num_char, -1, &multiplied_nums_rect, font_styles);
		}
	}

	DeleteObject(SelectObject(hdc, font_handle));
}

void main_window::on_command(int id){
	number_dialog numDlg;
	switch(id){
		case ID_FONT: 
			if (multiplication::choose_font()) {
				InvalidateRect(*this, 0, true);
				break;
			}
			break;
		case ID_NUMBER: 
			numDlg.do_modal(0, *this);
			InvalidateRect(*this, 0, true);
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
