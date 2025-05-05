#include "main.h"
#include "rc.h"
#include <stdexcept>

using namespace std;

int number_dialog::idd() const {
	return IDD_NUMBER; 
}
bool number_dialog::on_init_dialog(){
	this->set_int(IDC_EDIT1, range);
	return true;
}
bool number_dialog::on_ok(){
	try {
		range = this->get_real(IDC_EDIT1);
	}
	catch(std::runtime_error err) {
		const char * what = err.what();
		std::wstring msg(what, what + strlen(what));
		MessageBox(*this, msg.c_str(), _T("Error"), MB_OK | MB_ICONERROR);
	}
	
	return true;
}
 
void main_window::on_paint(HDC hdc){
	int range = nd.get_range();
	if (range <= 0) {
		return;
	}

	RECT parent_rect;
	GetClientRect(*this, &parent_rect);

	int height = parent_rect.bottom / (range + 1);
	int width = parent_rect.right / (range + 1);

	MoveToEx(hdc, 0, height, 0);
	LineTo(hdc, parent_rect.right, height);
	MoveToEx(hdc, width, 0, 0);
	LineTo(hdc, width, parent_rect.bottom);

	SetTextColor(hdc, this->font_color);

	auto old = SelectObject(hdc, this->font);

	for (int i = 0; i <= range; i++){
		for (int j = 0; j <= range; j++) {
			RECT r = { width * j, height * i, (width * j) + width, (height * i) + height };
			std::wstring wstr = (i == 0 || j == 0) ? std::to_wstring(i | j) : std::to_wstring(i * j);
			
			DrawText(hdc, wstr.c_str(), -1, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	SelectObject(hdc, old);
}

int number_dialog::get_range() {
	return range;
}

void main_window::on_command(int id){
	switch(id){
		case ID_FONT:
		{
			CHOOSEFONT cf;
			LOGFONT lf;

			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT
				| CF_SCREENFONTS | CF_EFFECTS;
			cf.hwndOwner = *this;
			cf.lpLogFont = &lf;

			auto hdc = ::GetDC(0);

			lf.lfHeight = -12 * ::GetDeviceCaps(hdc, LOGPIXELSY) / 72;

			if (ChooseFont(&cf)) {
				this->font = CreateFontIndirect(&lf);
				this->font_color = cf.rgbColors;
				InvalidateRect(*this, 0, TRUE);
			}
			break;
		}
		case ID_NUMBER:
			if (nd.do_modal(0, *this) == IDOK) {
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

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int)
{
	vsite::nwp::application app;
	main_window wnd;
	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (UINT_PTR)LoadMenu(hi, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
