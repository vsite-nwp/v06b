#include "main.h"
#include "rc.h"

int number_dialog::idd() const {
	return IDD_NUMBER; 
}
bool number_dialog::on_init_dialog(){
	return true;
}
bool number_dialog::on_ok(){
	return true;
}
 
void main_window::on_paint(HDC hdc){
	HFONT hFont = CreateFontIndirect(&lf);
	HGDIOBJ hOldFont = SelectObject(hdc, hFont);

	std::basic_string<TCHAR> s = _T("Tablica množenja");
	TextOut(hdc, 100, 100, s.c_str(), s.length());
	
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);
}

main_window::main_window() {
	lf = { 0 };
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
		case ID_NUMBER: 
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
