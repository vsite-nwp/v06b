#include "nwpwin.h"
#include "nwpdlg.h"

class number_dialog : public vsite::nwp::dialog
{
protected:
    int idd() const override;
    bool on_init_dialog() override;
    bool on_ok() override;

private:
    int number{ 5 };

public:
    int get_number() const { return number; }
    void set_number(int n) { number = n; }
};

class main_window : public vsite::nwp::window
{
protected:
    void on_paint(HDC hdc) override;
    void on_command(int id) override;
    void on_destroy() override;

private:
    int table_size{ 5 };
    LOGFONT font{ 0 };
    COLORREF color{ RGB(0, 0, 255) };
    bool custom_font_size{ false };

public:
    main_window();
};