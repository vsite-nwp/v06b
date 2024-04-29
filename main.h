#include "nwpwin.h"
#include "nwpdlg.h"

class number_dialog : public vsite::nwp::dialog {
	int inputNum;
protected:
	int idd() const override;
	bool on_init_dialog() override;
	bool on_ok() override;
};
 
class main_window : public vsite::nwp::window {
	LOGFONT lf;
public:
	main_window();
protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
};

// Selects new, stores old. On destruction selects old, deletes new.
class SelObj {
	HDC hdc;
	HGDIOBJ hOld;
public:
	SelObj(HDC, HGDIOBJ);
	~SelObj();
};
