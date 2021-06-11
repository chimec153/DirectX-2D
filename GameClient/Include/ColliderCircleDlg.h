#pragma once


// CColliderCircleDlg 대화 상자

class CColliderCircleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColliderCircleDlg)

public:
	CColliderCircleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColliderCircleDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COLLIDERCIRCLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_fR;
	afx_msg void OnEnChangeEditRadius();
	Vector3	m_vPos;
	afx_msg void OnEnChangeEditCenterX();
	afx_msg void OnEnChangeEditCenterY();
	afx_msg void OnEnChangeEditCenterZ();
	std::shared_ptr<class CComponent> m_pCC;
	int		m_iHeight;
	int		m_iWidth;

public:
	void SetCollider(std::shared_ptr<class CComponent> pCC);
	int GetHeight()	const;
	int GetWidth()	const;
	virtual BOOL OnInitDialog();
};
