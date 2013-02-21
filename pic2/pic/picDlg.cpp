// picDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pic.h"
#include "picDlg.h"
#include "cximage/ximage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CpicDlg 对话框


CpicDlg::CpicDlg(CWnd* pParent /*=NULL*/)
: CDialog(CpicDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpicDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CpicDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CpicDlg 消息处理程序

BOOL CpicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CpicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CpicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CpicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpicDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	HBITMAP handle;
	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,
		"几乎所有图片格式|*.jpg;*.jpeg;*.bmp;*.png;*.tif|.JPG|*.jpg;*.jpeg|.BMP|*.bmp|.PNG|*.PNG|.tif|*.tif;*.tiff|",
		this);

	CString strFileName;
	if(IDOK == 	dlg.DoModal())
	{
		strFileName = dlg.GetPathName();
	}//	if
	else 
		return;

	if((handle = Convert(strFileName)) < 0)
	{
		AfxMessageBox("加载图片失败:-(");
		return;
	}//	if

	if(SaveBmpFile("f:\\pic.bmp",handle))
		AfxMessageBox("pic.bmp已保存在F盘下:-)");
	else 
		AfxMessageBox("保存失败:-(");

	/*清理垃圾。*/
	::GlobalFree(handle);
	strFileName.ReleaseBuffer();
	m_image.Destroy();
}

bool CpicDlg::SaveBmpFile( CString str,HBITMAP handle )
{
	CFile file;
	BITMAP bitm;
	char* pBuffer;
	BITMAPINFOHEADER fileinfo;
	BITMAPINFO bmpinfo;
	CBitmap *bmp;

	/*open & create bitmap file.*/
	if(!file.Open(str,CFile::modeCreate|CFile::modeWrite))
		return false;

	bmp = CBitmap::FromHandle(handle);
	/*create bitmap objects.*/
	if(!bmp->GetBitmap(&bitm))
		return false;

	int ncharsize=bitm.bmHeight*bitm.bmWidthBytes;
	pBuffer=new char[ncharsize];
	memset(pBuffer,0xFF,ncharsize);

	bmpinfo.bmiHeader.biSize=40;
	bmpinfo.bmiHeader.biWidth=bitm.bmWidth;
	bmpinfo.bmiHeader.biHeight=bitm.bmHeight;
	bmpinfo.bmiHeader.biPlanes=bitm.bmPlanes;
	bmpinfo.bmiHeader.biBitCount=bitm.bmBitsPixel;
	bmpinfo.bmiHeader.biCompression=BI_RGB;

	int nline=GetDIBits(GetDC()->GetSafeHdc(),
		(HBITMAP)bmp->GetSafeHandle(),0,bmpinfo.bmiHeader.biHeight,pBuffer,&bmpinfo,DIB_RGB_COLORS);

	int nsize=nline*bitm.bmWidthBytes;

	char cc;
	cc=66;
	file.Write(&cc,1);

	cc=77;
	file.Write(&cc,1);

	long size=54+nsize;
	file.Write(&size,4);

	size=0;
	file.Write(&size,4);

	size=54;
	file.Write(&size,4);

	fileinfo.biSize=40;
	fileinfo.biWidth=bitm.bmWidth;
	fileinfo.biHeight=bitm.bmHeight;
	fileinfo.biPlanes=bitm.bmPlanes;
	fileinfo.biBitCount=bitm.bmBitsPixel;
	fileinfo.biCompression=BI_RGB;
	fileinfo.biSizeImage=0;
	fileinfo.biXPelsPerMeter=0;
	fileinfo.biYPelsPerMeter=0;
	fileinfo.biClrUsed=0;
	fileinfo.biClrImportant=0;

	file.Write(&fileinfo,fileinfo.biSize);
	file.Write(pBuffer,nsize);

	file.Close();
	bmp->DeleteObject();
	delete pBuffer;
	return true;
}

HBITMAP CpicDlg::Convert( CString strFileName )
{
	CBitmap *bmp;
	CDC * pDC;
	CDC memDC;
	CBrush brush;
	CFont f;

	BYTE * pBits;
	long  lSrcWidth,		//原图长与宽
		lSrcHeight,
		lLineBytes,			//原图每行总字节数
		lScanWidth;			//转换为8位图之后的宽度，必须是大于原图且为4的倍数
	int iStep,iCol,iRow,iBmWidth,iBmHeight,iFontSize,iMin;

	if(!m_image.Load(strFileName))
		return (HBITMAP)-1;

	pBits = m_image.GetBits();
	lSrcWidth = m_image.GetWidth();
	lSrcHeight = m_image.GetHeight();

	lLineBytes = ((lSrcWidth * 3) / 4) * 4;	//为了与8位位图数据对齐，原图每行总字节数也必须为4的倍数，
	if(lLineBytes < lSrcWidth * 3)			//在这里转换需要比原图每行总字节数大
		lLineBytes += 4;

#define STANDARDWIDTH		750.0
#define STANDARDFONTSIZE	70.0
#define STANDARDSTEP		10.2

	iMin = min(lSrcWidth,lSrcHeight);
	iFontSize = (iMin * STANDARDFONTSIZE) / STANDARDWIDTH;
	iStep = sqrt((iMin * iMin * STANDARDSTEP * STANDARDSTEP) / (STANDARDWIDTH * STANDARDWIDTH));

	iCol = lSrcWidth / iStep;
	iRow = lSrcHeight / iStep;
	iBmWidth = iCol * iStep;
	iBmHeight = iRow * iStep;

	pDC = GetDC();
	bmp = new CBitmap();
	bmp->CreateCompatibleBitmap(pDC,iBmWidth,iBmHeight);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(bmp);

	brush.CreateSolidBrush(RGB(255,255,255));
	memDC.FillRect(CRect(0,0,iBmWidth,iBmHeight),&brush);

	f.CreatePointFont(iFontSize,"微软雅黑");
	memDC.SelectObject(&f);

	for(int i=0; i<iRow; i++)
	{
		for(int j=0; j<iCol; j++)
		{
			int r,g,b,iColorTemp;
			r = g = b = 0;

			for(int ii=0; ii<iStep; ii++)
			{
				/*r，g，b的总大小。*/
				for(int jj=0; jj<iStep; jj++)
				{
					BYTE color[3]; 
					for(int s=0; s<3; s++)
						color[s]=pBits[lLineBytes * lSrcHeight - 
						(i * lLineBytes * iStep + 
						j * iStep * 3 + 
						ii * lLineBytes + 
						jj * 3 + s)];

					r += color[0];
					g += color[1];
					b += color[2];
				}//	for
			}//	for

			r /= iStep*iStep,
				g /= iStep*iStep,
				b /= iStep*iStep;

			iColorTemp = unsigned int(b * 0.299 + g * 0.587 + r * 0.114);	/*计算灰度。*/

			//TRACE(":%d %d %d\n",r,g,b);	
			//TRACE(":%d\n",iColorTemp);		

			CString str;
			/*有没有什么好办法干掉这么多的if？*/
			//if(iColorTemp <= 32)
			//	str = "囧";
			//else if(32 < iColorTemp && iColorTemp <= 64)
			//	str = "@";
			//else if(64 < iColorTemp && iColorTemp <= 96)
			//	str = "@";
			//else if(96 < iColorTemp && iColorTemp <= 128)
			//	str = "B";
			//else if(128 < iColorTemp && iColorTemp <= 160)
			//	str = "S";
			//else if(160 < iColorTemp && iColorTemp <= 192)
			//	str = "1";
			//else if(192 < iColorTemp && iColorTemp <= 224)
			//	str = "1";
			//else /*if(224<iColorTemp && iColorTemp<=256)*/
			//	str = "1";

			/*more efficient.*/
			switch(iColorTemp/32)
			{
			case 0:
				str = "囧";break;
			case 1:
				str = "@";break;
			case 2:
				str = "@";break;
			case 3:
				str = "B";break;
			case 4:
				str = "S";break;
			case 5:
				str = "1";break;
			case 6:
				str = "1";break;
			default:
				str = "1";
			}//	switch

			memDC.DrawText(str,str.GetLength(),
				CRect(j*iStep,
				i*iStep,
				j*iStep + iStep,
				i*iStep + iStep),
				DT_CENTER);

			str.ReleaseBuffer();
		}//	for
	}//	for

	//pDC->BitBlt(0,0,iBmWidth,iBmHeight,&memDC,0,0,SRCCOPY);
	//SaveBmpFile("f:\\pic.bmp",bmp);

	/*清理垃圾。*/
	brush.DeleteObject();
	f.DeleteObject();
	pDC->DeleteDC();
	memDC.DeleteDC();
	return (HBITMAP)bmp->GetSafeHandle();
}