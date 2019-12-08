/*----------------------------------------
   HEXCALC.C -- Hexadecimal Calculator
                (c) Charles Petzold, 1992
  ----------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdLine, int nCmdShow){

     static TCHAR szAppName [] = _T("HexCalc");
     HWND        hwnd ;
     MSG         msg;
     WNDCLASSEX    wndclass ;

     if (!hPrevInstance){
			  wndclass.cbSize = sizeof(WNDCLASSEX);
			  wndclass.style          = CS_HREDRAW | CS_VREDRAW;
			  wndclass.lpfnWndProc    = WndProc;
			  wndclass.cbClsExtra     = 0 ;
			  wndclass.cbWndExtra     = DLGWINDOWEXTRA ;
			  wndclass.hInstance      = hInstance ;
			  wndclass.hIcon          = (HICON)LoadImage(
				                                         NULL, 
				                                         MAKEINTRESOURCE(IDI_APPLICATION),
				                                         IMAGE_ICON,
				                                         0,
				                                         0,
				                                         LR_DEFAULTSIZE | LR_SHARED
				                                        );
			  wndclass.hIconSm        = LoadIcon(wndclass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
			  wndclass.hCursor        = LoadCursor (NULL, IDC_ARROW) ;
			  wndclass.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
			  wndclass.lpszMenuName   = NULL ;
			  wndclass.lpszClassName  = szAppName ;

			  if (!RegisterClassEx(&wndclass)) {
				  MessageBox(NULL,
					  _T("ÉEÉBÉìÉhÉEÇÃìoò^Ç…é∏îsÇµÇ‹ÇµÇΩ"),
					  _T("ÉEÉBÉìÉhÉEÇÃìoò^ÉGÉâÅ["),
					  NULL);
				  return 1;
			  }
	 }

     hwnd = CreateDialog (hInstance, szAppName, 0, NULL) ;

     ShowWindow (hwnd, nCmdShow) ;

     while (GetMessage (&msg, NULL, 0, 0)){
	      TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
	 }

     return msg.wParam ;
}

void ShowNumber (HWND hwnd, DWORD dwNumber){
	 TCHAR szBuffer[20];

     SetDlgItemText (hwnd, VK_ESCAPE, _tcsupr(_ltot(dwNumber, szBuffer, 16)) );
}

DWORD CalcIt (DWORD dwFirstNum, short nOperation, DWORD dwNum){

	switch (nOperation){
          case '=' : return dwNum ;
          case '+' : return dwFirstNum +  dwNum ;
          case '-' : return dwFirstNum -  dwNum ;
          case '*' : return dwFirstNum *  dwNum ;
          case '&' : return dwFirstNum &  dwNum ;
          case '|' : return dwFirstNum |  dwNum ;
          case '^' : return dwFirstNum ^  dwNum ;
          case '<' : return dwFirstNum << dwNum ;
          case '>' : return dwFirstNum >> dwNum ;
          case '/' : return dwNum ? dwFirstNum / dwNum : ULONG_MAX ;
          case '%' : return dwNum ? dwFirstNum % dwNum : ULONG_MAX ;
          default  : return 0L ;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
     static BOOL  bNewNumber = TRUE ;
     static DWORD dwNumber, dwFirstNum ;
     static short nOperation = '=' ;
     HWND         hButton ;

     switch (message){

          case WM_KEYDOWN:                   // left arrow --> backspace
               if (wParam != VK_LEFT)
                    break ;
               wParam = VK_BACK ;
                                             // fall through
          case WM_CHAR:
               if ((wParam = toupper (wParam)) == VK_RETURN)
                    wParam = '=' ;

               hButton = GetDlgItem (hWnd, wParam) ;

               if (hButton != NULL){
                    SendMessage (hButton, BM_SETSTATE, 1, 0L) ;
                    SendMessage (hButton, BM_SETSTATE, 0, 0L) ;
			   }
               else{
                    MessageBeep (0) ;
                    break ;
			   }
                                             // fall through
          case WM_COMMAND:
               SetFocus (hWnd) ;

               if (wParam == VK_BACK)                  // backspace
                    ShowNumber (hWnd, dwNumber /= 16) ;

               else if (wParam == VK_ESCAPE)           // escape
                    ShowNumber (hWnd, dwNumber = 0L) ;

               else if (isxdigit (wParam)){            // hex digit
                    if (bNewNumber){
                         dwFirstNum = dwNumber ;
                         dwNumber = 0L ;
					}
                    bNewNumber = FALSE ;

                    if (dwNumber <= ULONG_MAX >> 4)
                         ShowNumber (hWnd, dwNumber = 16 * dwNumber + wParam -
                              (isdigit (wParam) ? '0' : 'A' - 10)) ;
                    else
                         MessageBeep (0) ;
			   }
               else{                                   // operation
                    if (!bNewNumber)
                         ShowNumber (hWnd, dwNumber =
                              CalcIt (dwFirstNum, nOperation, dwNumber)) ;
                    bNewNumber = TRUE ;
                    nOperation = wParam ;
			   }
               return 0 ;

          case WM_DESTROY:
               PostQuitMessage (0) ;
               return 0 ;
	 }
     return DefWindowProc (hWnd, message, wParam, lParam);
}
