#pragma once

#include "../GameEngine.h"

	class CCollisionSection
	{
		friend class CSceneCollision;

	private:
		CCollisionSection();
		~CCollisionSection();

	private:
		int								m_iX;
		int								m_iY;
		int								m_iZ;
		int								m_iIdx;
		Vector3					m_vMin;
		Vector3					m_vMax;
		Vector3					m_vSize;
		std::list<class CCollider*>		m_ColliderList;

	public:
		bool Init(int x, int y, int z, int idx, Vector3 vSize);
		void SetCenter(const Vector3& vCenter);
		void AddCollider(class CCollider* pCol);
		bool MouseCollision(float fTime);
		void Collision(float fTime);
		void Clear();
		
	private:
		static bool SortZ(class CCollider* pSrc, class CCollider* pDst);
	};