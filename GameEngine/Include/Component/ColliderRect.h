#pragma once
#include "Collider.h"
	class CColliderRect :
		public CCollider
	{
	public:
		CColliderRect();
		CColliderRect(const CColliderRect& col);
		virtual ~CColliderRect();

	private:
		RectInfo			m_tInfo;
		Vector2				m_vExtend;

	public:
		void SetExtend(float x, float y);
		void SetExtend(const Vector2& v);
		const RectInfo& GetInfo()	const;
		const Vector2& GetExtent()	const;

	public:
		virtual bool Init();
		virtual void Start();
		virtual void Update(float fTime);
		virtual void PostUpdate(float fTime);
		virtual void Collision(float fTime);
		virtual void PreRender(float fTime);
		virtual void Render(float fTime);
		virtual void PostRender(float fTime);
		virtual std::shared_ptr<CComponent> Clone();

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);

	public:
		virtual bool Collision(class CCollider* pDest);
		virtual bool CollisionMouse(const Vector2& vMousePos);
	};