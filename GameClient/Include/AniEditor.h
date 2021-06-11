#pragma once


// CAniEditor 대화 상자


class CAniEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CAniEditor)

public:
	CAniEditor(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAniEditor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANIEDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditTag();
	CString m_strTag;
	CString m_strTex;
	afx_msg void OnEnChangeEditTexTag();
	afx_msg void OnBnClickedButtonTextureLoad();
	afx_msg void OnBnClickedButtonAddSequence();
	CListBox m_hList;
	afx_msg void OnLbnSelchangeListSequence();
	afx_msg void OnEnChangeEditRate();
	afx_msg void OnEnChangeEditTime();
	float m_fRate;
	float m_fMaxTime;
	afx_msg void OnBnClickedButtonSaveSequence();
	afx_msg void OnBnClickedButtonLoadSequence();
};
