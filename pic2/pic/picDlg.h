// picDlg.h : ͷ�ļ�
//

#pragma once
#include "cximage/ximage.h"

// CpicDlg �Ի���
class CpicDlg : public CDialog
{
// ����
public:
	CpicDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CxImage m_image;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();

	/*ת����������*/
	HBITMAP Convert(CString strFileName);

	/*����ͼƬ�ļ���*/
	bool SaveBmpFile(CString str,HBITMAP handle);
};
