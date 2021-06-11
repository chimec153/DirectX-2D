#pragma once

#include "Input.h"

	typedef struct _tagInputAxis
	{
		char											strTag[64];
		std::vector<std::function<void(float, float)>>	vecFunc;
		void* pObj;

		_tagInputAxis()
		{
			memset(strTag, 0, 64);
			pObj = nullptr;
		}
	}InputAxis, * PInputAxis;

	typedef struct _tagInputAction
	{
		char									strTag[64];
		std::vector<std::function<void(float)>>	vecFunc[KT_END];
		void* pObj;

		_tagInputAction()
		{
			memset(strTag, 0, 64);
			pObj = nullptr;
		}
	}InputAction, * PInputAction;

	class CInputObj
	{
		friend class CGameObject;

	private:
		CInputObj();
		~CInputObj();

	private:
		std::unordered_map<std::string, PInputAxis>		m_mapAxis;
		std::unordered_map<std::string, PInputAction>	m_mapAction;

	public:
		template <typename T>
		void AddBindAxis(const std::string& strTag, T* pObj, void(T::* pFunc)(float, float))
		{
			PBindAxis pAxis = GET_SINGLE(CInput)->FindAxisBind(strTag);

			if (!pAxis)
				return;

			PInputAxis pIAxis = FindAxis(strTag);

			if (!pIAxis)
			{
				pIAxis = new InputAxis;
				m_mapAxis.insert(std::make_pair(strTag, pIAxis));

				GET_SINGLE(CInput)->AddBindAxis<CInputObj>(strTag, this, &CInputObj::CallAxisFunc);
			}

			strcpy_s(pIAxis->strTag, strTag.c_str());
			pIAxis->pObj = pObj;
			pIAxis->vecFunc.push_back(std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2));
		}

	public:
		template <typename T>
		void AddBindAction(const std::string& strTag, T* pObj, void(T::* pFunc)(float), KEY_TYPE eType)
		{
			PBindAction pAction = GET_SINGLE(CInput)->FindActionBind(strTag);

			if (!pAction)
				return;

			PInputAction pIAction = FindAction(strTag);

			if (!pIAction)
			{
				pIAction = new InputAction;
				m_mapAction.insert(std::make_pair(strTag, pIAction));
				GET_SINGLE(CInput)->AddBindAction(strTag, this, &CInputObj::CallActionFunc, eType);
			}

			strcpy_s(pIAction->strTag, strTag.c_str());
			pIAction->pObj = pObj;
			pIAction->vecFunc[eType].push_back(bind(pFunc, pObj, std::placeholders::_1));
		}

	public:
		PInputAxis FindAxis(const std::string& strTag);
		PInputAction FindAction(const std::string& strTag);

	public:
		void CallAxisFunc(const std::string&, float fScale, float fTime);
		void CallActionFunc(const std::string&, KEY_TYPE eType, float fTime);
	};
