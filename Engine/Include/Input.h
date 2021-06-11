#pragma once

#include "GameEngine.h"

	enum KEY_TYPE
	{
		KT_DOWN,
		KT_DBLCLICK,
		KT_UP,
		KT_END
	};

	enum KEY_UNION
	{
		KU_CTRL,
		KU_ALT,
		KU_SHIFT,
		KU_END
	};

	enum class KEY_INPUT_TYPE
	{
		KIT_WINDOW,
		KIT_DINPUT
	};

	enum class MOUSE_TYPE
	{
		LEFT,
		RIGHT,
		WHEEL,
		END
	};

	typedef struct _tagKeyInfo
	{
		char	cKey;
		float	fTime;
		bool	bPush;

		_tagKeyInfo()
		{
			cKey = 0;
			fTime = 0.f;
			bPush = false;
		}
	}KeyInfo, * PKeyInfo;

	typedef struct _tagActionKey
	{
		PKeyInfo		pInfo;
		bool			bPush;

		_tagActionKey()
		{
			pInfo = nullptr;
			bPush = false;
		}

	}ActionKey, * PActionKey;

	typedef struct _tagAxisKey
	{
		PKeyInfo		pInfo;
		float			fScale;

		_tagAxisKey()
		{
			pInfo = nullptr;
			fScale = 0.f;
		}
	}AxisKey, * PAxisKey;

	typedef struct _tagObjBindAxisFunc
	{
		void* pObj;
		std::function<void(const char*, float, float)>	pFunc;

		_tagObjBindAxisFunc()
		{
			pObj = nullptr;
			pFunc = nullptr;
		}
	}ObjBindAxisFunc, * PObjBindAxisFunc;

	typedef struct _tagObjBindActionFunc
	{
		void* pObj;
		std::function<void(const char*, KEY_TYPE, float)>	pFunc;

		_tagObjBindActionFunc()
		{
			pObj = nullptr;
			pFunc = nullptr;
		}
	}ObjBindActionFunc, * PObjBindActionFunc;

	typedef struct _tagBindAxis
	{
		std::string										strTag;
		std::vector<PAxisKey>							vecAxisKey;
		float											fScale;
		std::vector<ObjBindAxisFunc>					vecFunc;

		_tagBindAxis()
		{
			fScale = 0.f;
		}

		~_tagBindAxis()
		{
			SAFE_DELETE_VECLIST(vecAxisKey);
		}
	}BindAxis, * PBindAxis;

	typedef struct _tagBindAction
	{
		std::string									strTag;
		std::vector<PActionKey>						vecActionKey;
		std::vector<ObjBindActionFunc>				vecFunc[KT_END];
		bool										bUnion[KU_END];

		_tagBindAction()
		{
			memset(&bUnion, 0, sizeof(bool) * KU_END);
		}

		~_tagBindAction()
		{
			SAFE_DELETE_VECLIST(vecActionKey);
		}
	}BindAction, * PBindAction;

	class CInput
	{
	private:
		IDirectInput8* m_pInput;
		IDirectInputDevice8* m_pKeyBoard;
		IDirectInputDevice8* m_pMouse;
		std::list<PKeyInfo>								m_KeyList;
		std::unordered_map<std::string, PBindAxis>		m_mapBindAxis;
		std::unordered_map<std::string, PBindAction>	m_mapBindAction;
		KEY_INPUT_TYPE									m_eInputType;
		unsigned char									m_cKeyInput[256];
		HWND											m_hWnd;
		Vector2											m_vMousePos;
		Vector2											m_vMouseWorldPos;
		Vector2											m_vPrevPos;
		Vector2											m_vMouseMove;
		DIMOUSESTATE									m_tMouseState;
		class CMouseObj* m_pMouseObj;
		bool											m_bShowMouse;
		bool											m_bDown[(int)MOUSE_TYPE::END];
		bool											m_bPress[(int)MOUSE_TYPE::END];
		bool											m_bUp[(int)MOUSE_TYPE::END];
		int												m_iMouseX;
		int												m_iMouseY;
		int												m_iWheel;
		bool											m_bWinActive;
		std::function<void(const std::wstring&)>		m_EnterFunc;
		std::function<void(const std::wstring&)>		m_WordFunc;
		std::wstring									m_strIME;

	public:
		void SetEnterFunc(void(*pFunc)(const std::wstring&));
		template <typename T>
		void SetEnterFunc(T* pObj, void(T::* pFunc)(const std::wstring&))
		{
			if (!pFunc)
			{
				m_EnterFunc = nullptr;
				return;
			}

			m_EnterFunc = std::bind(pFunc, pObj, std::placeholders::_1);
		}
		void SetWordFunc(void(*pFunc)(const std::wstring&));
		template <typename T>
		void SetWordFunc(T* pObj, void(T::* pFunc)(const std::wstring&))
		{
			if (!pFunc)
			{
				m_WordFunc = nullptr;
				return;
			}

			m_WordFunc = std::bind(pFunc, pObj, std::placeholders::_1);
		}
		void EnterIME(const std::wstring& str);
		void InputWord(const std::wstring& str);
		const std::wstring& GetWord()	const;

	public:
		Vector2 GetMousePos()	const;
		Vector2 GetMouseWorldPos()	const;
		Vector2 GetMouseMove()	const;
		class CMouseObj* GetMouse()	const;
		int GetMoveX()	const;
		int GetMoveY()	const;
		void SetShow(bool bShow);
		bool GetShow()	const;
		int GetWheel()	const;
		void SetWinActive(bool bAct);

	public:
		bool Init(HINSTANCE hInst, HWND hWnd);
		bool InitWindow();
		bool InitDInput();
		void Update(float fTime);
		void Collision(float fTime);
		void PreRender(float fTime);
		void Render(float fTime);
		bool IsDown(MOUSE_TYPE eType);
		bool IsUp(MOUSE_TYPE eType);
		bool IsPress(MOUSE_TYPE eType);

	public:
		void UpdateDMouse(float fTime);
		void UpdateDKeyBoard(float fTime);
		void UpdateWindow(float fTime);
		void UpdateDInput(float fTime);
		void UpdateAxis(float fTime);
		void UpdateAction(float fTime);
		void UpdateMouse(float fTime);
		void CreateMouse();

	public:
		void AddAxisKey(const std::string& strTag, char cKey, float fScale);
		void AddBindAxis(const std::string& strTag, void(*pFunc)(const std::string&, float, float));
		template <typename T>
		void AddBindAxis(const std::string& strTag, T* pObj, void(T::* pFunc)(const std::string&, float, float))
		{
			PBindAxis pAxis = FindAxisBind(strTag);

			if (!pAxis)
			{
				pAxis = new BindAxis;
				pAxis->strTag = strTag;
				m_mapBindAxis.insert(std::make_pair(strTag, pAxis));
			}

			ObjBindAxisFunc tAxF;
			tAxF.pFunc = std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			tAxF.pObj = pObj;
			pAxis->vecFunc.push_back(tAxF);
		}

		void DeleteBindAxis(const std::string& strTag);
		void DeleteBindAxis(const std::string& strTag, void* pObj);

		void AddActionKey(const std::string& strTag, char cKey);
		void AddActionUnion(const std::string& strTag, KEY_UNION eUnion, char cKey);
		void AddBindAction(const std::string& strTag, void(*pFunc)(const std::string&, KEY_TYPE, float), KEY_TYPE eType);
		void DeleteBindAction(const std::string& strTag);
		void DeleteBindAction(const std::string& strTag, void* pObj);

		template <typename T>
		void AddBindAction(const std::string& strTag, T* pObj, void(T::* pFunc)(const std::string&, KEY_TYPE, float), KEY_TYPE eType)
		{
			PBindAction pAction = FindActionBind(strTag);

			if (!pAction)
			{
				pAction = new BindAction;
				pAction->strTag = strTag;
				m_mapBindAction.insert(std::make_pair(strTag, pAction));
			}

			ObjBindActionFunc tAcF;

			tAcF.pFunc = std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			tAcF.pObj = pObj;

			pAction->vecFunc[eType].push_back(tAcF);
		}

	public:
		PBindAxis FindAxisBind(const std::string& strTag)
		{
			std::unordered_map<std::string, PBindAxis>::iterator iter = m_mapBindAxis.find(strTag);

			if (iter == m_mapBindAxis.end())
				return nullptr;

			return iter->second;
		}

		PBindAction FindActionBind(const std::string& strTag)
		{
			std::unordered_map<std::string, PBindAction>::iterator iter = m_mapBindAction.find(strTag);

			if (iter == m_mapBindAction.end())
				return nullptr;

			return iter->second;
		}

		unsigned char Convert(unsigned char);

		DECLARE_SINGLE(CInput)
	};
