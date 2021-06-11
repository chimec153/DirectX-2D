#pragma once


// CCameraDlg 대화 상자

class CCameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditAngle();
	float m_fAngle;
	float m_fDist;
	afx_msg void OnEnChangeEditDist();
	int m_iType;
	afx_msg void OnEnChangeEditType();

private:
	int		m_iHeight;
	int		m_iWidth;
	std::shared_ptr<class CComponent>	m_pCom;

public:
	int GetWidth()	const;
	int GetHeight()	const;

	void SetComponent(std::shared_ptr<class CComponent> pCom);
	virtual BOOL OnInitDialog();
};
