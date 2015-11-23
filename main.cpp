#include "main.h"
#include "rc.h"
const float djelitelj = 1.5;

int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, (broj - 1));
	return true;
}
bool NumberDialog::OnOK(){
	try
	{
		NumberDialog::broj = GetInt(IDC_EDIT1) + 1;
		return true;
	}
	catch (XCtrl e)
	{
		return false;
	}
}

 
void MainWindow::OnPaint(HDC hdc)
{
	RECT rect;
	GetClientRect(*this, &rect);
	HFONT hFont = CreateFontIndirect(&lf);
	HGDIOBJ holdfont = SelectObject(hdc, hFont);

	int sirina = (rect.right / broj);
	int visina = (rect.bottom / broj);

	int tempSir;
	int tempVis;

	#pragma region Top Header Linije

	MoveToEx(hdc, 0, visina / djelitelj, NULL);
	LineTo(hdc, 0, visina / djelitelj);
	LineTo(hdc, rect.right, visina / djelitelj);

	#pragma endregion 

	#pragma region Left Header Linije

	MoveToEx(hdc, sirina / djelitelj, 0, NULL);
	LineTo(hdc, sirina / djelitelj, 0);
	LineTo(hdc, sirina / djelitelj, rect.bottom);

	#pragma endregion 

	#pragma region Top Header brojevi

	tempSir = sirina;

	int headVisina = visina / 4;

	for (int i = 1; i < broj; i++)
	{
		std::basic_string<TCHAR> s = std::to_string(i);
		TextOut(hdc, tempSir, headVisina, s.c_str(), s.length());
		tempSir += sirina;
	}

	#pragma endregion 

	#pragma region Left Header brojevi

	tempVis = visina;

	int headSirina = sirina / 4;

	for (int i = 1; i < broj; i++)
	{
		std::basic_string<TCHAR> s = std::to_string(i);
		TextOut(hdc, headSirina, tempVis, s.c_str(), s.length());
		tempVis += visina;
	}

	#pragma endregion 

	#pragma region Tablica množenja	

	tempSir = sirina;
	tempVis = visina;

	for (int i = 1; i < broj; i++)
	{
		for (int j = 1; j < broj; j++)
		{			
			std::basic_string<TCHAR> s = std::to_string((i * j));
			TextOut(hdc, tempSir, tempVis, s.c_str(), s.length());
			tempSir += sirina;
		}
		tempVis += visina;
		tempSir = sirina;
	}

	#pragma endregion 
	
	DeleteObject(holdfont);	
}

void MainWindow::OnCommand(int id){		
	NumberDialog ndl;	
	switch(id){
		case ID_FONT: 
			lf = { 0 };
			strcpy(lf.lfFaceName, "Arial");
			CHOOSEFONT cf;
			ZeroMemory(&cf, sizeof cf);
			cf.lStructSize = sizeof cf;
			cf.Flags = CF_INITTOLOGFONTSTRUCT
				| CF_SCREENFONTS | CF_EFFECTS;
			cf.lpLogFont = &lf;
			if (ChooseFont(&cf)) CreateFontIndirect(&lf);
			InvalidateRect(*this, NULL, TRUE);			
			break;
		case ID_NUMBER: 
			ndl.broj = broj;
			if (ndl.DoModal(NULL, *this) == IDOK)
			{				
				broj = ndl.broj;
			}
			InvalidateRect(*this, NULL, TRUE);
			break;
		case ID_EXIT: 
			DestroyWindow(*this); 
			break;
	}
}

void MainWindow::OnDestroy(){
	::PostQuitMessage(0);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MainWindow wnd;
	
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP", 
		(int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	

	return app.Run();
}
