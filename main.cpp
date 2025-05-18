#include "nwpwin.h"
#include "nwpdlg.h"
#include "main.h"
#include "rc.h"

#include <iostream>
#include <string>
#include <sstream>
#include <format>

tstring to_tstring(int value) {
    return std::format(_T("{}"), value);
}

int number_dialog::idd() const
{
    return IDD_NUMBER;
}

bool number_dialog::on_init_dialog()
{
    SetDlgItemInt(*this, IDC_EDIT1, number, FALSE);
    return true;
}

bool number_dialog::on_ok()
{
    BOOL success;
    int input = GetDlgItemInt(*this, IDC_EDIT1, &success, FALSE);
    if (!success || input < 1 || input > 15) {
        MessageBox(*this, _T("Broj izmedu 1 i 15"), _T("Error"), MB_ICONERROR);
        return false;
    }
    number = input;
    return true;
}

main_window::main_window()
{
    _tcscpy(font.lfFaceName, _T("Arial"));
    font.lfWeight = FW_MEDIUM;

    //Def font
    HDC hdc = GetDC(nullptr);
    font.lfHeight = -MulDiv(11, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ReleaseDC(nullptr, hdc);
}

void main_window::on_paint(HDC hdc)
{
    RECT rect;
    GetClientRect(*this, &rect);

    //Velicina polja
    double width = rect.right / (table_size + 1.0);
    double height = rect.bottom / (table_size + 1.0);

    //Omjer polje/font
    int target_text_height = static_cast<int>(height * 0.8);

    LOGFONT dynamic_font = font;
    int font_size = 8;  //minimalna
    int actual_text_height = 0;

    if (!custom_font_size) 
    {

        HFONT hfont = nullptr;
        for (font_size = 8; font_size <= 50; ++font_size)
        {
            dynamic_font.lfHeight = -MulDiv(font_size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            hfont = CreateFontIndirect(&dynamic_font);
            HGDIOBJ old_font = SelectObject(hdc, hfont);

            //Velicina fonta
            RECT text_rect = { 0, 0, 0, 0 };
            tstring sample_text = _T("ABCD1234");
            DrawText(hdc, sample_text.c_str(), -1, &text_rect, DT_CALCRECT | DT_SINGLELINE);

            actual_text_height = text_rect.bottom - text_rect.top;

            SelectObject(hdc, old_font);
            DeleteObject(hfont);

            //Max
            if (actual_text_height >= target_text_height || font_size == 50)
                break;
        }

        dynamic_font.lfHeight = -MulDiv(font_size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    }

    HFONT hfont = CreateFontIndirect(&dynamic_font);
    HGDIOBJ old_font = SelectObject(hdc, hfont);

    //Crtanje polja
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(220, 220, 220));
    HGDIOBJ old_pen = SelectObject(hdc, pen);

    for (int i = 0; i <= table_size + 1; ++i) 
    {
        MoveToEx(hdc, static_cast<int>(i * width), 0, nullptr);
        LineTo(hdc, static_cast<int>(i * width), rect.bottom);

        MoveToEx(hdc, 0, static_cast<int>(i * height), nullptr);
        LineTo(hdc, rect.right, static_cast<int>(i * height));
    }

    //Crtanje brojeva
    SetBkMode(hdc, TRANSPARENT);
    for (int i = 0; i <= table_size; ++i) 
    {
        for (int j = 0; j <= table_size; ++j) 
        {
            RECT cell = { j * width, i * height, (j + 1) * width, (i + 1) * height };
            tstring text;
            if (i == 0 && j == 0) 
            {
                text = _T("x*y");
                SetTextColor(hdc, RGB(255, 0, 0));
            }
            else if (i == 0) 
            {
                text = to_tstring(j);
                SetTextColor(hdc, RGB(255, 0, 0));
            }
            else if (j == 0) 
            {
                text = to_tstring(i);
                SetTextColor(hdc, RGB(255, 0, 0));
            }
            else 
            {
                text = to_tstring(i * j);
                SetTextColor(hdc, color);
            }
            DrawText(hdc, text.c_str(), -1, &cell, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    SelectObject(hdc, old_pen);
    DeleteObject(pen);

    SelectObject(hdc, old_font);
    DeleteObject(hfont);
}

void main_window::on_command(int id)
{
    switch (id) 
    {
    case ID_FONT:
    {
        CHOOSEFONT cf = { sizeof(CHOOSEFONT) };
        cf.hwndOwner = *this;
        cf.lpLogFont = &font;
        cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS;
        cf.rgbColors = color;
        if (ChooseFont(&cf)) 
        {
            color = cf.rgbColors;
            custom_font_size = true;
            InvalidateRect(*this, nullptr, TRUE);
        }
        break;
    }
    case ID_NUMBER: 
    {
        number_dialog dlg;
        dlg.set_number(table_size);
        if (dlg.do_modal(nullptr, *this) == IDOK) 
        {
            table_size = dlg.get_number();
            InvalidateRect(*this, nullptr, TRUE);
        }
        break;
    }
    case ID_EXIT:
        ::DestroyWindow(*this);
        break;
    }
}

void main_window::on_destroy()
{
    ::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
    vsite::nwp::application app;
    main_window wnd;
    wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));
    return app.run();
}