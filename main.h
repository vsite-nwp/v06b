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
	CHOOSEFONT font_specs;
	LOGFONT font;
	main_window() {
		HDC device_context = GetDC(0);
		ZeroMemory(&font, sizeof(font));
		wcsncpy_s(font.lfFaceName, _T("Times New Roman"), sizeof("Times New Roman"));
		font.lfHeight = -12 * (GetDeviceCaps(device_context, LOGPIXELSY) / 72);
		ReleaseDC(*this, device_context);
		ZeroMemory(&font_specs, sizeof(font_specs));
		font_specs.lStructSize = sizeof(font_specs);

	}

	bool choose_font(LOGFONT& log_font_ref, COLORREF& color_ref) {
		//Prepare font specs for LOGFONT 
		font_specs.hwndOwner = *this;
		font_specs.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
		font_specs.lpLogFont = &log_font_ref;
		font_specs.rgbColors = color_ref;

		if (ChooseFont(&font_specs)) {
			color_ref = font_specs.rgbColors;
			return true;
		}
		return false;
	}

protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
};
