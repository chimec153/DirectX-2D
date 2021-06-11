#pragma once



// CColliderInfo 보기

class CColliderInfo : public CDialogEx
{
	DECLARE_DYNCREATE(CColliderInfo)

public:
	CColliderInfo();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CColliderInfo();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDER_RECT};
#endif

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditTop();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnEnChangeEditLeft();
	RectInfo	m_tInfo;
	Vector2		m_tExtent;
	std::shared_ptr<class CComponent>	m_pRC;
	afx_msg void OnEnChangeEditRight();
	afx_msg void OnEnChangeEditBot();
	afx_msg void OnEnChangeEditExx();
	afx_msg void OnEnChangeEditExy();
	void SetCollider(std::shared_ptr<class CComponent> pRC);

	int		m_iHeight;
	int		m_iWidth;

public:
	int GetHeight()	const;
	int GetWidth()	const;
	virtual BOOL OnInitDialog();
};


