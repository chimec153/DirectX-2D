#pragma once

class CObjCreate : public CDialogEx
{
	DECLARE_DYNAMIC(CObjCreate)

public:
	CObjCreate(CWnd* pParent = nullptr);
	virtual ~CObjCreate();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJ_CREATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	void AddProtoType(const std::string& strKey);
	void ClearProto();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonCreate();
	CListBox m_ProtoList;
	afx_msg void OnLbnSelchangeListProto();
	afx_msg void OnBnClickedButtonOpenTilemap();

	class CTileMapDlg* m_pTileMapDlg;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
