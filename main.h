#include "nwpwin.h"
#include "nwpdlg.h"

class number_dialog : public vsite::nwp::dialog {
public:
	int num_to_multiply_to = 1;
protected:
	int idd() const override;
	bool on_init_dialog() override;
	bool on_ok() override;
};
 
class main_window : public vsite::nwp::window {
public:
	int limit_num;
	COLORREF font_color = RGB(0, 0, 0);
	LOGFONT font;
	main_window() {
		ZeroMemory(&font, sizeof(font));
		wcsncpy(font.lfFaceName, _T("Times New Roman"), sizeof("Times New Roman"));
	}
protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
};
