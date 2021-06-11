#pragma once


// CSpriteDlg 대화 상자

class CSpriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpriteDlg)

public:
	CSpriteDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSpriteDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SPRITE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bStop;
	afx_msg void OnBnClickedCheckStop();
	BOOL m_bLoop;
	afx_msg void OnBnClickedCheckLoop();
	BOOL m_bArrayTex;
	afx_msg void OnBnClickedCheckArraytex();
	afx_msg void OnLbnSelchangeListSeq();
private:
	int		m_iHeight;
	int		m_iWidth;
	std::shared_ptr<class CComponent>	m_pCom;

public:
	int GetWidth()	const;
	int GetHeight()	const;

	void SetComponent(std::shared_ptr<class CComponent> pCom);
	virtual BOOL OnInitDialog();
	CString m_strCurrent;
	afx_msg void OnEnChangeEditCurrent();
	afx_msg void OnEnChangeEditFrame();
	int m_iFrame;
	float m_fTime;
	int m_iMaxFrame;
	afx_msg void OnEnChangeEditSeqTime();
	CListBox m_SeqList;
	afx_msg void OnEnChangeEditFrame2();
	afx_msg void OnEnChangeEditFrameMax();
};
