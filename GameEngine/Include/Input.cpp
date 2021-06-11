#include "Input.h"
#include "Device.h"
#include "CameraManager.h"
#include "Component/Camera.h"
#include "UI/MouseObj.h"
#include "Scene/SceneManager.h"
#include "UI/UITransform.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	m_pInput(nullptr),
	m_pKeyBoard(nullptr),
	m_pMouse(nullptr),
	m_hWnd(0),
	m_tMouseState(),
	m_pMouseObj(nullptr),
	m_bShowMouse(true),
	m_iMouseX(0),
	m_iMouseY(0),
	m_iWheel(0),
	m_bWinActive(true)
{
	memset(m_cKeyInput, 0, sizeof(char) * 256);
	memset(m_bDown, 0, (int)MOUSE_TYPE::END);
	memset(m_bPress, 0, (int)MOUSE_TYPE::END);
	memset(m_bUp, 0, (int)MOUSE_TYPE::END);
}

CInput::~CInput()
{
	SAFE_DELETE_VECLIST(m_KeyList);
	SAFE_DELETE_MAP(m_mapBindAction);
	SAFE_DELETE_MAP(m_mapBindAxis);

	SAFE_RELEASE(m_pInput);

	if (m_pKeyBoard)
		m_pKeyBoard->Unacquire();

	if (m_pMouse)
		m_pMouse->Unacquire();

	SAFE_RELEASE(m_pKeyBoard);
	SAFE_RELEASE(m_pMouse);
	SAFE_RELEASE(m_pMouseObj);
}

void CInput::AddAxisKey(const std::string& strTag, char cKey, float fScale)
{
	PBindAxis pAxis = FindAxisBind(strTag);

	if (!pAxis)
	{
		pAxis = new BindAxis;
		pAxis->strTag = strTag;
		m_mapBindAxis.insert(std::make_pair(strTag, pAxis));
	}

	PKeyInfo pInfo = nullptr;

	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->cKey == cKey)
		{
			pInfo = *iter;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new KeyInfo;
		pInfo->cKey = cKey;
		m_KeyList.push_back(pInfo);
	}

	size_t iSize = pAxis->vecAxisKey.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pAxis->vecAxisKey[i]->pInfo->cKey == pInfo->cKey)
			return;
	}

	PAxisKey pKey = new AxisKey;

	pKey->pInfo = pInfo;
	pKey->fScale = fScale;
	pAxis->vecAxisKey.push_back(pKey);
}

void CInput::AddBindAxis(const std::string& strTag, void(*pFunc)(const std::string& strTag, float, float))
{
	PBindAxis pAxis = FindAxisBind(strTag);

	if (!pAxis)
	{
		pAxis = new BindAxis;
		pAxis->strTag = strTag;
		m_mapBindAxis.insert(std::make_pair(strTag, pAxis));		
	}

	ObjBindAxisFunc tAxF;

	tAxF.pFunc = std::bind(pFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	pAxis->vecFunc.push_back(tAxF);
}

void CInput::DeleteBindAxis(const std::string& strTag)
{
	std::unordered_map<std::string, PBindAxis>::iterator iter = m_mapBindAxis.find(strTag);

	if (iter == m_mapBindAxis.end())
		return;

	SAFE_DELETE(iter->second);

	m_mapBindAxis.erase(iter);
}

void CInput::DeleteBindAxis(const std::string& strTag, void* pObj)
{
	PBindAxis pAxis = FindAxisBind(strTag);

	if (!pAxis)
		return;

	size_t iSize = pAxis->vecFunc.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pAxis->vecFunc[i].pObj == pObj)
		{
			std::vector<ObjBindAxisFunc>::iterator iter = pAxis->vecFunc.begin() + i;
			pAxis->vecFunc.erase(iter);
			return;
		}
	}
}

void CInput::AddActionKey(const std::string& strTag, char cKey)
{
	PBindAction pAction = FindActionBind(strTag);

	if (!pAction)
	{
		pAction = new BindAction;
		pAction->strTag = strTag;
		m_mapBindAction.insert(std::make_pair(strTag, pAction));
	}

	PKeyInfo pInfo = nullptr;

	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->cKey == cKey)
		{
			pInfo = *iter;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new KeyInfo;
		pInfo->cKey = cKey;
		m_KeyList.push_back(pInfo);
	}

	size_t iSize = pAction->vecActionKey.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pAction->vecActionKey[i]->pInfo->cKey == pInfo->cKey)
			return;
	}

	PActionKey pKey = new ActionKey;

	pKey->pInfo = pInfo;

	pAction->vecActionKey.push_back(pKey);
}

void CInput::AddActionUnion(const std::string& strTag, KEY_UNION eUnion, char cKey)
{
	PBindAction pAction = FindActionBind(strTag);

	if (!pAction)
	{
		pAction = new BindAction;
		pAction->strTag = strTag;
		m_mapBindAction.insert(std::make_pair(strTag, pAction));
	}

	PKeyInfo pInfo = nullptr;

	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->cKey == cKey)
		{
			pInfo = *iter;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new KeyInfo;
		pInfo->cKey = cKey;
		m_KeyList.push_back(pInfo);
	}

	PActionKey pKey = nullptr;

	size_t iSize = pAction->vecActionKey.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pAction->vecActionKey[i]->pInfo->cKey == pInfo->cKey)
		{
			pAction->bUnion[eUnion] = true;
			break;
		}
	}

	if (pAction->vecActionKey.empty())
	{
		PActionKey pKey = new ActionKey;

		pKey->pInfo = pInfo;

		pAction->vecActionKey.push_back(pKey);

		pAction->bUnion[eUnion] = true;
	}
}

void CInput::AddBindAction(const std::string& strTag, void(*pFunc)(const std::string&, KEY_TYPE, float), KEY_TYPE eType)
{
	PBindAction pAction = FindActionBind(strTag);

	if (!pAction)
	{
		pAction = new BindAction;
		pAction->strTag = strTag;
		m_mapBindAction.insert(std::make_pair(strTag, pAction));
	}

	ObjBindActionFunc tOAcF;

	tOAcF.pFunc = std::bind(pFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	pAction->vecFunc[eType].push_back(tOAcF);
}

void CInput::DeleteBindAction(const std::string& strTag)
{
	std::unordered_map<std::string, PBindAction>::iterator iter = m_mapBindAction.find(strTag);

	if (iter == m_mapBindAction.end())
		return;

	SAFE_DELETE(iter->second);

	m_mapBindAction.erase(iter);
}

void CInput::DeleteBindAction(const std::string& strTag, void* pObj)
{
	PBindAction pAction = FindActionBind(strTag);

	if (!pAction)
		return;

	for (int i = 0; i < KT_END; ++i)
	{
		size_t iSize = pAction->vecFunc[i].size();

		for (size_t j = 0; j < iSize; ++j)
		{
			if (pAction->vecFunc[i][j].pObj == pObj)
			{
				std::vector<ObjBindActionFunc>::iterator iter = pAction->vecFunc[i].begin() + j;
				pAction->vecFunc[i].erase(iter);
				return;
			}
		}
	}
}

unsigned char CInput::Convert(unsigned char cKey)
{
	if (m_eInputType == KEY_INPUT_TYPE::KIT_DINPUT)
	{
		switch (cKey)
		{
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case '0':
			return DIK_0;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case '=':
			return DIK_EQUALS;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case 'Q':
			return DIK_Q;
		case 'W':
			return DIK_W;
		case 'E':
			return DIK_E;
		case 'R':
			return DIK_R;
		case 'T':
			return DIK_T;
		case 'Y':
			return DIK_Y;
		case 'U':
			return DIK_U;
		case 'I':
			return DIK_I;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case '[':
			return DIK_LBRACKET;
		case ']':
			return DIK_RBRACKET;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_CONTROL:
			return DIK_LCONTROL;
		case 'A':
			return DIK_A;
		case 'S':
			return DIK_S;
		case 'D':
			return DIK_D;
		case 'F':
			return DIK_F;
		case VK_SPACE:
			return DIK_SPACE;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case ':':
			return DIK_SEMICOLON;
		case '\'':
			return DIK_APOSTROPHE;
		case '`':
			return DIK_GRAVE;
		case VK_SHIFT:
			return DIK_LSHIFT;
		case '\\':
			return DIK_BACKSLASH;
		case 'Z':
			return DIK_Z;
		case 'X':
			return DIK_X;
		case 'C':
			return DIK_C;
		case 'V':
			return DIK_V;
		case 'B':
			return DIK_B;
		case 'N':
			return DIK_N;
		case 'M':
			return DIK_M;
		case ',':
			return DIK_COMMA;
		case '.':
			return DIK_PERIOD;
		case '/':
			return DIK_SLASH;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_LMENU:
			return DIK_LMENU;
		case VK_CAPITAL:
			return DIK_CAPITAL;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_RMENU:
			return DIK_RMENU;
		case VK_HOME:
			return DIK_HOME;
		case VK_UP:
			return DIK_UP;
		case VK_PRIOR:
			return DIK_PRIOR;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_END:
			return DIK_END;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_NEXT:
			return DIK_NEXT;
		case VK_INSERT:
			return DIK_INSERT;
			/*
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;*/
		}
	}

	return cKey;
}

void CInput::SetEnterFunc(void(*pFunc)(const std::wstring&))
{
	if (!pFunc)
	{
		m_EnterFunc = nullptr;
	}

	m_EnterFunc = std::bind(pFunc, std::placeholders::_1);
}

void CInput::SetWordFunc(void(*pFunc)(const std::wstring&))
{
	if (!pFunc)
	{
		m_WordFunc = nullptr;
		return;
	}

	m_WordFunc = std::bind(pFunc, std::placeholders::_1);
}

void CInput::EnterIME(const std::wstring& str)
{
	if (m_EnterFunc)
	{
		m_EnterFunc(str);
	}
	m_strIME = L"";
}

void CInput::InputWord(const std::wstring& str)
{
	if (m_WordFunc)
	{
		m_WordFunc(str);
	}

	m_strIME = str;
}

const std::wstring& CInput::GetWord() const
{
	return m_strIME;
}

Vector2 CInput::GetMousePos() const
{
	return Vector2(m_vMousePos.x * RATIO.x, m_vMousePos.y * RATIO.y);
}

Vector2 CInput::GetMouseWorldPos() const
{
	return Vector2(m_vMouseWorldPos.x * RATIO.x, m_vMouseWorldPos.y * RATIO.y);
}

Vector2 CInput::GetMouseMove() const
{
	return m_vMouseMove;
}

CMouseObj* CInput::GetMouse() const
{
	return m_pMouseObj;
}

int CInput::GetMoveX() const
{
	return m_iMouseX;
}

int CInput::GetMoveY() const
{
	return m_iMouseY;
}

void CInput::SetShow(bool bShow)
{
	m_bShowMouse = bShow;

	ShowCursor(bShow);
}

bool CInput::GetShow() const
{
	return m_bShowMouse;
}

int CInput::GetWheel() const
{
	return m_iWheel;
}

void CInput::SetWinActive(bool bAct)
{
	m_bWinActive = bAct;
}

bool CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	m_hWnd = hWnd;

	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&m_pInput, 0);

	m_eInputType = KEY_INPUT_TYPE::KIT_DINPUT;

	if (FAILED(hr))
		m_eInputType = KEY_INPUT_TYPE::KIT_WINDOW;

	switch (m_eInputType)
	{
	case KEY_INPUT_TYPE::KIT_WINDOW:
		InitWindow();
		break;
	case KEY_INPUT_TYPE::KIT_DINPUT:
		InitDInput();
		break;
	}

	POINT pt;

	GetCursorPos(&pt);

	m_vMousePos = pt;
/*
	AddBindAction("MouseDown", this, &CInput::MouseLDown, KT_DOWN);
	AddBindAction("MouseUp", this, &CInput::MouseLUp, KT_UP);*/

	return true;
}

bool CInput::InitWindow()
{
	return true;
}

bool CInput::InitDInput()
{
	if(FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return false;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return false;

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	return true;
}

void CInput::Update(float fTime)
{
	switch (m_eInputType)
	{
	case KEY_INPUT_TYPE::KIT_WINDOW:
		UpdateWindow(fTime);
		break;
	case KEY_INPUT_TYPE::KIT_DINPUT:
		if (m_bWinActive)
		{
			UpdateDMouse(fTime);
			UpdateDKeyBoard(fTime);
			UpdateDInput(fTime);
		}
		break;
	}
	
	UpdateAction(fTime);
	UpdateAxis(fTime);
	UpdateMouse(fTime);
}

void CInput::Collision(float fTime)
{
	m_pMouseObj->Collision(fTime);
}

void CInput::PreRender(float fTime)
{
	m_pMouseObj->CheckFrustum();
	m_pMouseObj->PreRender(fTime);
}

void CInput::Render(float fTime)
{
	m_pMouseObj->Render(fTime);

	m_iMouseX = 0;
	m_iMouseY = 0;
	m_iWheel = 0;
}

bool CInput::IsDown(MOUSE_TYPE eType)
{
	return m_bDown[(int)eType];
}

bool CInput::IsUp(MOUSE_TYPE eType)
{
	return m_bUp[(int)eType];
}

bool CInput::IsPress(MOUSE_TYPE eType)
{
	return m_bPress[(int)eType];
}

void CInput::UpdateDMouse(float fTime)
{
	HRESULT hr = m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			m_pMouse->Acquire();
	}

	m_iMouseX = (int)(m_tMouseState.lX / RATIO.x);
	m_iMouseY = (int)(m_tMouseState.lY / RATIO.y);

	m_iWheel = m_tMouseState.lZ;

	for (int i = 0; i < (int)MOUSE_TYPE::END; ++i)
	{
		if (m_tMouseState.rgbButtons[i] & 0x80)
		{
			if (!m_bDown[i] && !m_bPress[i])
			{
				m_bUp[i] = false;
				m_bDown[i] = true;
			}

			else if (!m_bPress[i])
			{
				m_bDown[i] = false;
				m_bPress[i] = true;
			}
		}

		else
		{
			if (m_bPress[i] && !m_bUp[i])
			{
				m_bDown[i] = false;
				m_bPress[i] = false;
				m_bUp[i] = true;
			}

			else if(m_bUp[i])
			{
				m_bUp[i] = false;
			}
		}
	}
}

void CInput::UpdateDKeyBoard(float fTime)
{
	HRESULT hr = m_pKeyBoard->GetDeviceState(256, m_cKeyInput);

	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			m_pKeyBoard->Acquire();
	}
}

void CInput::UpdateWindow(float fTime)
{
	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (GetAsyncKeyState((*iter)->cKey) & 0x8000)
		{
			(*iter)->bPush = true;
			(*iter)->fTime += fTime;
		}

		else
		{
			(*iter)->bPush = false;
			(*iter)->fTime = 0.f;
		}
	}
}

void CInput::UpdateDInput(float fTime)
{
	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (m_cKeyInput[(*iter)->cKey] & 0x80)
		{
			(*iter)->bPush = true;
			(*iter)->fTime += fTime;
		}

		else
		{
			(*iter)->bPush = false;
			(*iter)->fTime = 0.f;
		}
	}
}

void CInput::UpdateAxis(float fTime)
{
	std::unordered_map<std::string, PBindAxis>::iterator iter = m_mapBindAxis.begin();
	std::unordered_map<std::string, PBindAxis>::iterator iterEnd = m_mapBindAxis.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->fScale = 0;

		size_t iSize = iter->second->vecAxisKey.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			if (iter->second->vecAxisKey[i]->pInfo->bPush)
			{
				iter->second->fScale += iter->second->vecAxisKey[i]->fScale;
			}
		}

		size_t iFuncSize = iter->second->vecFunc.size();

		for (size_t i = 0; i < iFuncSize; ++i)
			iter->second->vecFunc[i].pFunc(iter->first.c_str(), iter->second->fScale, fTime);
	}
}

void CInput::UpdateAction(float fTime)
{
	char	cKey[KU_END] = { VK_CONTROL, VK_MENU, VK_SHIFT };
	bool	bUnion[KU_END] = {};
		switch (m_eInputType)
		{
		case KEY_INPUT_TYPE::KIT_WINDOW:
			for (int i = 0; i < KU_END; ++i)
			{
				if (GetAsyncKeyState(cKey[i]) & 0x8000)
					bUnion[i] = true;
			}
			break;
		case KEY_INPUT_TYPE::KIT_DINPUT:
			cKey[KU_CTRL] = DIK_LCONTROL;
			cKey[KU_ALT] = DIK_LALT;
			cKey[KU_SHIFT] = DIK_LSHIFT;

			for (int i = 0; i < KU_END; ++i)
			{
				if (m_cKeyInput[bUnion[i]] & 0x80)
					bUnion[i] = true;
			}
			break;
		}

	std::unordered_map<std::string, PBindAction>::iterator iter = m_mapBindAction.begin();
	std::unordered_map<std::string, PBindAction>::iterator iterEnd = m_mapBindAction.end();

	for (; iter != iterEnd; ++iter)
	{
		bool bEnable = true;

		for (int i = 0; i < KU_END; ++i)
		{
			if (iter->second->bUnion[i] != bUnion[i])
			{
				bEnable = false;
				break;
			}
		}

		if (!bEnable)
			continue;

		size_t iSize = iter->second->vecActionKey.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			if (iter->second->vecActionKey[i]->pInfo->bPush)
			{
				if (!iter->second->vecActionKey[i]->bPush)
				{
					iter->second->vecActionKey[i]->bPush = true;

					size_t iFuncSize = iter->second->vecFunc[KT_DOWN].size();

					for (size_t i = 0; i < iFuncSize; ++i)
						iter->second->vecFunc[KT_DOWN][i].pFunc(iter->first.c_str(), KT_DOWN, fTime);
				}
			}

			else if (iter->second->vecActionKey[i]->bPush)
			{
				iter->second->vecActionKey[i]->bPush = false;

				size_t iFuncSize = iter->second->vecFunc[KT_UP].size();

				for (size_t i = 0; i < iFuncSize; ++i)
					iter->second->vecFunc[KT_UP][i].pFunc(iter->first.c_str(), KT_UP, fTime);
			}
		}
	}
}

void CInput::UpdateMouse(float fTime)
{
	POINT pt;

	GetCursorPos(&pt);

	ScreenToClient(m_hWnd, &pt);

	RECT tRC = {};

	GetClientRect(m_hWnd,&tRC);
	
		if (pt.x < 0 || pt.x > tRC.right - tRC.left ||
			pt.y < 0 || pt.y > tRC.bottom - tRC.top)
		{
			if (!m_bShowMouse)
			{
				SetShow(!m_bShowMouse);
			}
		}

		else
		{
			if (m_bShowMouse)
			{
				SetShow(!m_bShowMouse);
			}
		}
	

	Vector2 vRatio = GET_SINGLE(CDevice)->GetWindowRatio();

	Vector2 vMousePos = pt;

	vMousePos /= vRatio;

	vMousePos.y = RESOLUTION.iHeight - vMousePos.y;

	m_vMousePos = vMousePos;

	m_vMouseMove = m_vMousePos - m_vPrevPos;

	m_vPrevPos = m_vMousePos;

	//SetCursorPos(500, 500);

	GetCursorPos(&pt);

	ScreenToClient(m_hWnd, &pt);

	m_vPrevPos = pt;
	m_vPrevPos /= vRatio;

	m_vPrevPos.y = RESOLUTION.iHeight - m_vPrevPos.y;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Resolution tRS = RESOLUTION;
	Vector3 vCamPos = {};
	if (pCam)
	{
		vCamPos = pCam->GetWorldPos();

		vCamPos.x -= (float)tRS.iWidth * pCam->GetPivot().x;
		vCamPos.y -= (float)tRS.iHeight * pCam->GetPivot().y;
	}

	m_vMouseWorldPos = m_vMousePos + Vector2(vCamPos.x, vCamPos.y);

	m_pMouseObj->SetRelativePos(m_vMousePos.x, m_vMousePos.y, 0.f);

	m_pMouseObj->Update(fTime);
	m_pMouseObj->PostUpdate(fTime);
}

void CInput::CreateMouse()
{
	SAFE_DELETE(m_pMouseObj);

	m_pMouseObj = new CMouseObj;

	m_pMouseObj->m_pScene = GET_SINGLE(CSceneManager)->GetScene();

	m_pMouseObj->m_pLayer = m_pMouseObj->m_pScene->FindLayer(ALBEDO_LAYER);

	if (!m_pMouseObj->Init())
	{
		SAFE_RELEASE(m_pMouseObj);
		return;
	}

	m_pMouseObj->SetRelativePos(m_vMousePos.x, m_vMousePos.y, 0.f);

	//m_bShowMouse = false;

	//ShowCursor(FALSE);

	memset(m_bDown, 0, (int)MOUSE_TYPE::END);
	memset(m_bPress, 0, (int)MOUSE_TYPE::END);
	memset(m_bUp, 0, (int)MOUSE_TYPE::END);
}