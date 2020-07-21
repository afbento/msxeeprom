#include <windows.h>
#include <stdio.h>
#include "main.h"

int mouseX,mouseY,mouseW,mouseL;
BOOL key[256], rkey[256],lbutton,rlbutton,bQuit=FALSE;

int islc,cslc;

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		case WM_MOUSEMOVE:	
	
         mouseX = LOWORD(lParam); 
         mouseY = HIWORD(lParam); 
         break; 
         
        case WM_MOUSEWHEEL:	
		mouseW = GET_WHEEL_DELTA_WPARAM(wParam);
         //mouseW = LOWORD(lParam); 
        // mouseY = HIWORD(lParam); 
         break; 
         
        case WM_LBUTTONDOWN:
        if(mouseL==0) mouseL=1;
    	
    	break;
    	
    	case WM_LBUTTONUP:
    	mouseL=0;
    	break;
         
        case WM_KEYDOWN:
     	key[wParam] = TRUE;
     	break;
     	
     	case WM_KEYUP:
			key[wParam] = FALSE;rkey[wParam]=FALSE;
        break;
		 
 		case WM_COMMAND:
			switch(LOWORD(wParam)) {   
		case CM_FILE_OPEN:
			DoFileOpenSave(hwnd);
			break;    
		case CM_ROM_CHECK:
			DoFileOpenCheck(hwnd);
			break; 
		case CM_FILE_EXIT:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
			
		case CM_ABOUT:
			MessageBox (NULL, "Visualizador Gráfico de Imagens para MSX (ROM,DSK,etc)                   (C)2020 Fábio A. Bento" , "About...", 0);
			break;
			
			}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

FILE *arqrom;
int i,i1,j,j1,k,romsize,endr,m,mx,my,ac,romload,romcheck,e_ardu,dly_ardu,erro;
unsigned char c,r,v;

unsigned char romdata[5000000];

static HWND texto_teste;

int xsize=820,ysize=500;

BOOL DoFileOpenSave(HWND hwnd) {
	char str[300];
	
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	szFileName[0] = 0;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.ROM)\0*.rom\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "rom";
	ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		if(GetOpenFileName(&ofn)) {
			arqrom=fopen(szFileName,"rb");
	
			fseek (arqrom , 0 , SEEK_END);
    		romsize = ftell (arqrom);
    		rewind (arqrom);
	
			i=0;j=0;
	
			while(i<romsize) {
				fread(&romdata[i],1,1,arqrom);
				i++;
				}
 
			fclose(arqrom);
			
			endr=0;
			sprintf(str,"Visualizador de ROM em binário (PROG) - %s ",szFileName);
			SetWindowTextA(hwnd,str);
			
			
			//rom swap
			if(romsize==32768) {
				for(i=0;i<16384;i++) {
					j=romdata[i+16384];
					romdata[i+16384]=romdata[i];
					romdata[i]=j;
				}
				
			}
			
			romload=2;e_ardu=0;erro=0;
		}
	
	return TRUE;
}

BOOL DoFileOpenCheck(HWND hwnd) {
	char str[300];
	
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	szFileName[0] = 0;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.ROM)\0*.rom\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "rom";
	ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		if(GetOpenFileName(&ofn)) {
			arqrom=fopen(szFileName,"rb");
	
			fseek (arqrom , 0 , SEEK_END);
    		romsize = ftell (arqrom);
    		rewind (arqrom);
	
			i=0;j=0;
	
			while(i<romsize) {
				fread(&romdata[i],1,1,arqrom);
				i++;
				}
 
			fclose(arqrom);
			
			endr=0;
			sprintf(str,"Visualizador de ROM em binário (CHECK) - %s ",szFileName);
			
			SetWindowTextA(hwnd,str);
			romload=0;e_ardu=0;romcheck=1;erro=0;
		}
	
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; 
	HWND hwnd; 
	MSG msg; 

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; 
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.lpszMenuName  = "MAINMENU";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Envia ROM para arduino",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		1100, /* width */
		590, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	char *text = (char*)malloc(256); 
	char str[100];
	
	COLORREF pix[xsize*ysize];
	
	COLORREF c1,c2;
	
	HDC hdc;
   	hdc = GetDC(hwnd);
   	
   	HBITMAP map;
   	
   	FILE *ComPort;
   	
   	char str1[100],str2[100],str3[1];
   	
   	HANDLE hSerial;                          // Handle to the Serial port
	char  ComPortName[] = "\\\\.\\COM3";  // Name of the Serial port(May Change) to be opened,
	
	unsigned long byter;
   
   	HDC hdcMem;
   	
   	DCB dcb;
   	char name[100], COM[4];
   	
   	HFONT hFont;
   	hFont = CreateFont(15,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,0/*ANTIALIASED_QUALITY*/, VARIABLE_PITCH,"LCD Ultra");

/*
	texto_teste = CreateWindow(TEXT("Edit"), "00000", WS_CHILD | WS_VISIBLE | WS_BORDER,
                30, 180, 70, 20, hwnd,NULL,NULL, NULL);*/
	

	
	endr=0;
	memset(&romdata,0,sizeof(romdata));
	romsize=4920;
	
	  
	/*
  	dcb.BaudRate = CBR_9600;
 	dcb.ByteSize = 8;
  	dcb.Parity = 0;
  	dcb.StopBits = 1;
  	dcb.fDtrControl =  DTR_CONTROL_ENABLE;
  	
   
  	ComPort = fopen(ComPortName,"rw");

  	if(ComPort)
  	{ 
         SetCommState(ComPort, &dcb);
         
    }*/
    
    hSerial = CreateFile(ComPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	GetCommState(hSerial, &dcbSerialParams);
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	SetCommState(hSerial, &dcbSerialParams);
	
	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(hSerial, &timeouts);
	
	//sleep(1);
	
//	sprintf(str1,"%d,%d",0,8);
//	WriteFile(hSerial, &str1,5, &byter, NULL);
    
    romload=0;romcheck=0;
	
//	while(GetMessage(&msg, NULL, 0, 0)) { 
   while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {

	if((mouseX>10)&&(mouseX<(xsize+10))&&(mouseY>10)&&(mouseY<(ysize+10))) {
		mx=(mouseX-10)/(9*3);
		my=(mouseY-10)/3;
		m=mx*164+my;
		
	}
	
	if(mouseL==1) {islc=m;mouseL=2;} 
	
	
	
	memset(&pix,0,sizeof(pix));
	j=-9;
	for(i=0;i<4920;i++) {
		
		
		if(((mouseL==2)&&(((i>=islc)&&(i<=m))||((i<=islc)&&(i>=m))))||((mouseL==0)&&(i==m))) 
			{c1=RGB(150,150,255);c2=RGB(0,0,140); } else {c1=RGB(255,255,255);c2=RGB(0,0,0); }
		
		i1=i+endr;
		k=i%164;
		if(k==0) j+=9;
		if((romdata[i1]&1)==1) pix[k*xsize+j+7]=c1; else pix[k*xsize+j+7]=c2;
		if((romdata[i1]&2)==2) pix[k*xsize+j+6]=c1; else pix[k*xsize+j+6]=c2;
		if((romdata[i1]&4)==4) pix[k*xsize+j+5]=c1; else pix[k*xsize+j+5]=c2;
		if((romdata[i1]&8)==8) pix[k*xsize+j+4]=c1; else pix[k*xsize+j+4]=c2;
		if((romdata[i1]&16)==16) pix[k*xsize+j+3]=c1; else pix[k*xsize+j+3]=c2;
		if((romdata[i1]&32)==32) pix[k*xsize+j+2]=c1; else pix[k*xsize+j+2]=c2;
		if((romdata[i1]&64)==64) pix[k*xsize+j+1]=c1; else pix[k*xsize+j+1]=c2;
		if((romdata[i1]&128)==128) pix[k*xsize+j+0]=c1; else pix[k*xsize+j+0]=c2;
		}
		
	ac=164;	
	if(key[VK_SHIFT]) ac=1; else if(key[VK_CONTROL]) ac=164*4;
	endr-=(mouseW/80)*ac;
	mouseW=0;
	if(endr<0) endr=0;
	if(endr>(romsize-4920))	endr=romsize-4920;
	
	
	SelectObject(hdc,hFont);
  	sprintf(str,"Endereço:      ");
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,100,str,strlen(str));
   	
   	sprintf(str,"0x%02x      ",endr);
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,115,str,strlen(str));
   	
   	sprintf(str,"Endereço no cursor:      ");
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,150,str,strlen(str));
   	sprintf(str,"0x%02x      ",endr+m);
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,165,str,strlen(str));
   	
   	sprintf(str,"Valor do byte no cursor:      ");
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,190,str,strlen(str));
   	sprintf(str,"0x%02x      ",romdata[endr+m]);
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,205,str,strlen(str));
   	
  // 	if((hSerial)&&(romload==2)) {
   	//	ReadFile(hSerial,&str2,1,&byter,NULL);
   	//	if(str2) romload=1;
	//   }
   	
   	if((hSerial)&&((romload>0)||(romcheck>0))/*&&(GetCommState(ComPort, &dcb))*/) {
	
	//for(i=0;i<32768;i++) {
	//GetCommState(ComPort, &dcb);
	//if(dly_ardu>1) {
   //	fprintf(ComPort,"%d %d \n",e_ardu,romdata[e_ardu]);
   //	dly_ardu=0;
  
//	for(e_ardu=0;e_ardu<32768;e_ardu++) {
	if(romload==1) ReadFile(hSerial,&str2,1,&byter,NULL);
	
	
	if((str2[0]=='G')||(romload==2)) {	
		sprintf(str1,"G%d,%d         ",e_ardu,romdata[e_ardu]);
		WriteFile(hSerial, &str1,strlen(str1), &byter, NULL); 
		e_ardu++;
		romload=1;}
	
	
	
	if(str2[0]=='E') {erro++;e_ardu++;}

	if(e_ardu>=32768) {romload=0;romcheck=0;} //}
	
//	dly_ardu++;
   	
    //}
   	//fscanf(ComPort,"%s %s %s %d %s %d",&str1,&str1,&str1,&i,&str1,&i);
   	
   	if(romcheck==1) {
   		sprintf(str1,"L%d ",e_ardu);
		WriteFile(hSerial, &str1,strlen(str1), &byter, NULL);
		
		i=0;str3[0]="";
		while(str3[0]!='\n') {
			ReadFile(hSerial,&str3,1,&byter,NULL);
			str2[i]=str3[0];
			i++;
		}
		
		i=atoi(str2);
		if(i!=romdata[e_ardu]) erro++;
		e_ardu++;
		
	   }
   	
   	sprintf(str,"End.=%d dado=%d erros=%d    ",e_ardu-1,romdata[e_ardu-1],erro);
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,225,str,strlen(str));
   	
   	sprintf(str,"End.=%d erchk=%d %s %s   ",romdata[e_ardu],i,str1,str2);
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,245,str,strlen(str));
   //	TextOutA(hdc,850,245,str2,strlen(str2));
   	//sleep(1);
   }
   	
  /* 	if(mouseL==2) sprintf(str,"mousel=%d     ",abs(m-islc)+1); else sprintf(str,"                         ");
   	SetTextAlign(hdc,TA_LEFT);
   	TextOutA(hdc,850,405,str,strlen(str));*/
   
   	map=CreateBitmap(xsize,ysize,1,8*4,(void *)pix);

	hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem,map);
	//BitBlt(hdc,10,10,800,500,hdcMem,0,0,SRCCOPY);
	StretchBlt(hdc,10,10,xsize,ysize,hdcMem,0,0,xsize/3,ysize/3,SRCCOPY);
	DeleteObject(map);

	DeleteDC(hdcMem);
	
	
//		TranslateMessage(&msg); 
//		DispatchMessage(&msg); 
	}
}
	return msg.wParam;
}
