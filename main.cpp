#include "main.h"
#include "rc.h"

int NumberDialog::IDD(){
	return IDD_NUMBER; 
}
bool NumberDialog::OnInitDialog(){
	return true;
}
bool NumberDialog::OnOK(){
	int edNum = GetInt(IDC_EDIT1);
	return true;
}

 
void MainWindow::OnPaint(HDC hdc){
	RECT rct; GetClientRect(*this, &rct);

	//setting coordinate system
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetViewportExtEx(hdc, rct.right, rct.bottom, NULL);
	SetWindowExtEx(hdc, numNumbers, numNumbers, NULL);

	//drawing lines for table edges
	MoveToEx(hdc, 1, 0, 0);
	LineTo(hdc, 1, rct.bottom);
	MoveToEx(hdc, 0, 1, 0);
	LineTo(hdc, rct.right, 1);
	
	//table drawing
	//cntRow->cords for Y (Line we are positioned) ::::::::: cntClmn->cords for x (column we are positioned)
	for (int cntRow = 0; cntRow <= numNumbers; ++cntRow) {
		for (int cntClmn = 1; cntClmn <= numNumbers; ++cntClmn) {
			/*HRGN rgn = CreateRectRgn(cntClmn - 1, cntRow, cntClmn, cntRow + 1);
			SelectClipRgn(hdc, rgn);
			TCHAR bfr;
			RECT rgnRect; GetClientRect(*this, &rgnRect);
			sprintf(&bfr, _T("%d"), cntClmn);
			DrawText(hdc, &bfr, sizeof bfr, &rgnRect, DT_CENTER);*/
		}
	}
}

void MainWindow::OnCommand(int id){
	switch(id){
		case ID_FONT:
			CHOOSEFONT chf;
			chf.Flags = CF_SCREENFONTS|CF_EFFECTS|CF_INITTOLOGFONTSTRUCT;
			ChooseFont(&chf);
			break;
		case ID_NUMBER: {
			NumberDialog numDlg;
			numDlg.DoModal(NULL, *this);
			numNumbers = numDlg.edNum;
			InvalidateRect(*this, 0, TRUE);
			break;
		}
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
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"),
		(int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.Run();
}
