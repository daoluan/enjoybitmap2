// picDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pic.h"
#include "picDlg.h"
#include "cximage/ximage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CpicDlg �Ի���


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


// CpicDlg ��Ϣ�������

BOOL CpicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpicDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HBITMAP handle;
	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,
		"��������ͼƬ��ʽ|*.jpg;*.jpeg;*.bmp;*.png;*.tif|.JPG|*.jpg;*.jpeg|.BMP|*.bmp|.PNG|*.PNG|.tif|*.tif;*.tiff|",
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
		AfxMessageBox("����ͼƬʧ��:-(");
		return;
	}//	if

	if(SaveBmpFile("f:\\pic.bmp",handle))
		AfxMessageBox("pic.bmp�ѱ�����F����:-)");
	else 
		AfxMessageBox("����ʧ��:-(");

	/*����������*/
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
	long  lSrcWidth,		//ԭͼ�����
		lSrcHeight,
		lLineBytes,			//ԭͼÿ�����ֽ���
		lScanWidth;			//ת��Ϊ8λͼ֮��Ŀ�ȣ������Ǵ���ԭͼ��Ϊ4�ı���
	int iStep,iCol,iRow,iBmWidth,iBmHeight,iFontSize,iMin;

	if(!m_image.Load(strFileName))
		return (HBITMAP)-1;

	pBits = m_image.GetBits();
	lSrcWidth = m_image.GetWidth();
	lSrcHeight = m_image.GetHeight();

	lLineBytes = ((lSrcWidth * 3) / 4) * 4;	//Ϊ����8λλͼ���ݶ��룬ԭͼÿ�����ֽ���Ҳ����Ϊ4�ı�����
	if(lLineBytes < lSrcWidth * 3)			//������ת����Ҫ��ԭͼÿ�����ֽ�����
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

	f.CreatePointFont(iFontSize,"΢���ź�");
	memDC.SelectObject(&f);

	for(int i=0; i<iRow; i++)
	{
		for(int j=0; j<iCol; j++)
		{
			int r,g,b,iColorTemp;
			r = g = b = 0;

			for(int ii=0; ii<iStep; ii++)
			{
				/*r��g��b���ܴ�С��*/
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

			iColorTemp = unsigned int(b * 0.299 + g * 0.587 + r * 0.114);	/*����Ҷȡ�*/

			//TRACE(":%d %d %d\n",r,g,b);	
			//TRACE(":%d\n",iColorTemp);		

			CString str;
			/*��û��ʲô�ð취�ɵ���ô���if��*/
			//if(iColorTemp <= 32)
			//	str = "��";
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
				str = "��";break;
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

	/*����������*/
	brush.DeleteObject();
	f.DeleteObject();
	pDC->DeleteDC();
	memDC.DeleteDC();
	return (HBITMAP)bmp->GetSafeHandle();
}