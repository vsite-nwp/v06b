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
	int limit_num = 1;
	COLORREF font_color = RGB(0, 0, 0);
	LOGFONT font;

	main_window() {
		HDC device_context = GetDC(0);
		ZeroMemory(&font, sizeof(font));
		wcsncpy_s(font.lfFaceName, _T("Times New Roman"), sizeof("Times New Roman"));
		font.lfHeight = -14 * GetDeviceCaps(device_context, LOGPIXELSY) / 72;
		ReleaseDC(*this, device_context);
	}

	bool choose_font() {
		//Prepare font specs for LOGFONT 
		CHOOSEFONT font_specs;
		LOGFONT font_copy;

		font_specs.lStructSize = sizeof(font_specs);
		font_specs.lpLogFont = &font_copy;
		font_specs.hwndOwner = *this;
		font_specs.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
		font_specs.rgbColors = font_color;
		font_copy = font;

		if (ChooseFont(&font_specs)) {
			font = font_copy;
			font_color = font_specs.rgbColors;
			return true;
		}
		return false;
	}

protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
};
