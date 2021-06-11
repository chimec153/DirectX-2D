﻿#pragma once


// CUISliderDlg 대화 상자

class CUISliderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUISliderDlg)

public:
	CUISliderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUISliderDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SLIDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
private:
	int		m_iHeight;
	int		m_iWidth;
	std::shared_ptr<class CComponent>	m_pCom;

public:
	int GetWidth()	const;
	int GetHeight()	const;

	void SetComponent(std::shared_ptr<class CComponent> pCom);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};