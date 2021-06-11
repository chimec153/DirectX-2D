#pragma once



// CWorldObj 보기

class CWorldObj : public CFormView
{
	DECLARE_DYNCREATE(CWorldObj)

protected:
	CWorldObj();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CWorldObj();

public:
	void AddObject(const std::string& strTag);
	void DeleteObject(const std::string& strTag);

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORLDOBJ};
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	std::vector<TreeItem>	m_vecItem;

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()
public:
	HTREEITEM		m_RootItem;
	afx_msg void OnTvnSelchangedTreeControl(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void DoDataExchange(CDataExchange* pDX);
	CTreeCtrl m_RootItemtest;
	afx_msg void OnNMClickTreeControl(NMHDR* pNMHDR, LRESULT* pResult);
};


