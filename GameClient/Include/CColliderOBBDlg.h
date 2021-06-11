#pragma once


// CColliderOBBDlg 대화 상자

class CColliderOBBDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColliderOBBDlg)

public:
	CColliderOBBDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColliderOBBDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COLLIDER_OBB2D };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	Vector3	m_vPos;
	Vector3	m_vAxis[AXIS_END];
	float	m_fLength[AXIS_END];
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnChangeEditAxisXX();
	afx_msg void OnEnChangeEditAxisXY();
	afx_msg void OnEnChangeEditAxisXZ();
	afx_msg void OnEnChangeEditAxisYX();
	afx_msg void OnEnChangeEditAxisYY();
	afx_msg void OnEnChangeEditAxisYZ();
	afx_msg void OnEnChangeEditAxisZX();
	afx_msg void OnEnChangeEditAxisZY();
	afx_msg void OnEnChangeEditAxisZZ();
	afx_msg void OnEnChangeEditLengthX();
	afx_msg void OnEnChangeEditLengthY();
	afx_msg void OnEnChangeEditLengthZ();
	std::shared_ptr<class CComponent> m_pOC;
	int		m_iHeight;
	int		m_iWidth;

public:
	int GetHeight()	const;
	int GetWidth()	const;
	void SetCollider(std::shared_ptr<class CComponent> pOC);
	virtual BOOL OnInitDialog();
};
