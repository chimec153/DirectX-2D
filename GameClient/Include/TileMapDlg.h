#pragma once


// CTileMap 대화 상자

class CTileMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTileMapDlg)

public:
	CTileMapDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTileMapDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TILEMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboTile();
	afx_msg void OnCbnSelchangeComboTileIndex();
	afx_msg void OnCbnSelchangeComboTileOption();
	afx_msg void OnBnClickedCheckModify();
	BOOL m_bModify;
	int m_iTileHeight;
	TILE_OPTION	m_eOption;
	TILE_TYPE	m_eType;
	int	m_iIdx;
	std::shared_ptr<class CComponent> m_pTileMap;
	afx_msg void OnEnChangeEditHeight();
	void SetTileMap(std::shared_ptr<class CComponent> pCom);
	CComboBox m_TileCombo;
	CComboBox m_IndexCombo;
	CComboBox m_OptionCombo;
	virtual BOOL OnInitDialog();
	int		m_iWidth;
	int		m_iHeight;
	Vector2	m_vTileSize;
	int		m_iCountX;
	int		m_iCountY;

public:
	int GetWidth()	const;
	int GetHeight()	const;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	float m_fSizex;
	afx_msg void OnEnChangeEditTileSizeX();
	afx_msg void OnEnChangeEditTileSizeY();
	afx_msg void OnEnChangeEditTileCntX();
	afx_msg void OnEnChangeEditTileCntY();
	afx_msg void OnBnClickedButtonCreateTile();
	afx_msg void OnEnChangeEditTileTexture();
	CString m_strTex;
	afx_msg void OnBnClickedButtonLoadTileTexture();
};
