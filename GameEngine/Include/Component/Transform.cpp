#include "Transform.h"
#include "../Device.h"
#include "../Resource/ShaderManager.h"

CTransform::CTransform()	:
	m_pScene(nullptr),
	m_pOwner(nullptr),
	m_pParent(nullptr),
	m_bInheritScale(true),
	m_bInheritRotX(true),
	m_bInheritRotY(true),
	m_bInheritRotZ(true),
	m_bUpdateScale(true),
	m_bUpdateRot(true),
	m_bUpdatePos(true)
{
	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i];
		m_vWorldAxis[i] = Vector3::Axis[i];
	}

	m_vMeshSize = Vector3(1.f, 1.f, 1.f);
}

CTransform::CTransform(const CTransform& transform)
{
	*this = transform;

	m_vecChild.clear();
	m_pScene = nullptr;
	m_pOwner = nullptr;
	m_pParent = nullptr;

	m_bUpdateScale = true;
	m_bUpdateRot = true;
	m_bUpdatePos = true;
}

CTransform::~CTransform()
{
}

void CTransform::Update(float fTime)
{
}

void CTransform::PostUpdate(float fTime)
{
	if (m_bUpdateScale)
		m_matScale.Scaling(m_vWorldScale);

	if (m_bUpdateRot)
		m_matRot.Rotation(m_vWorldRot);

	if (m_bUpdatePos)
		m_matPos.Translate(m_vWorldPos);

	m_matWorld = m_matScale * m_matRot * m_matPos;
}

void CTransform::SetTransform()
{
	Resolution tRS = RESOLUTION;

	m_tCBuffer.matWorld = m_matWorld;
	m_tCBuffer.matView = DirectX::XMMatrixIdentity();
	m_tCBuffer.matProj = DirectX::XMMatrixOrthographicOffCenterLH(-tRS.iWidth / 2.f, tRS.iWidth / 2.f, 
		-tRS.iHeight / 2.f, tRS.iHeight / 2.f, 0.f, 3000.f);
	m_tCBuffer.matWV = m_matWorld;
	m_tCBuffer.matWVP = m_tCBuffer.matWorld * m_tCBuffer.matProj;
	m_tCBuffer.vPivot = m_vPivot;
	m_tCBuffer.vMeshSize = m_vMeshSize;

	m_tCBuffer.matWorld.Transpose();
	m_tCBuffer.matProj.Transpose();
	m_tCBuffer.matWV.Transpose();
	m_tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);
}

CTransform* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::InheritScale()
{
	if (m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::InheritRot()
{
	if (m_bInheritRotX)
		m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

	if (m_bInheritRotY)
		m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

	if (m_bInheritRotZ)
		m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;

	if (m_pParent)
		InheritPos();

	m_bUpdateRot = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::InheritPos()
{
	Matrix tMat;

	tMat.Rotation(m_pParent->GetWorldRot());

	memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float)*3);

	m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::SetRelativeScale(const Vector3& v)
{
	m_vVelocityScale += v - m_vRelativeScale;

	m_vRelativeScale = v;

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x,y,z) - m_vRelativeScale;

	m_vRelativeScale = Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::SetRelativeRot(const Vector3& v)
{
	m_vVelocityRot += v - m_vRelativeRot;

	m_vRelativeRot = v;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetRelativeRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z) - m_vRelativeRot;

	m_vRelativeRot = Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetRelativeRotX(float x)
{
	m_vVelocityRot.x += x - m_vRelativeRot.x;

	m_vRelativeRot.x = x;

	m_vWorldRot.x = m_vRelativeRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetRelativeRotY(float y)
{
	m_vVelocityRot.y += y - m_vRelativeRot.y;

	m_vRelativeRot.y = y;

	m_vWorldRot.y = m_vRelativeRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetRelativeRotZ(float z)
{
	m_vVelocityRot.z += z - m_vRelativeRot.z;

	m_vRelativeRot.z = z;

	m_vWorldRot.z = m_vRelativeRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetRelativePos(const Vector3& v)
{
	m_vVelocity += v - m_vRelativePos;

	m_vRelativePos = v;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::SetRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z) - m_vRelativePos;

	m_vRelativePos = Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);
	}	

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::AddRelativeScale(const Vector3& v)
{
	m_vVelocityScale += v;

	m_vRelativeScale += v;

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z);

	m_vRelativeScale += Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::AddRelativeRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vRelativeRot += v;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotX)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}		

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddRelativeRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z);

	m_vRelativeRot += Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotX)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddRelativeRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vRelativeRot.x += x;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotX)
		m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddRelativeRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vRelativeRot.y += y;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotY)
		m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddRelativeRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vRelativeRot.z += z;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotZ)
		m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddRelativePos(const Vector3& v)
{
	m_vVelocity += v;

	m_vRelativePos += v;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::AddRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z);

	m_vRelativePos += Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

Vector3 CTransform::GetRelativeScale() const
{
	return m_vRelativeScale;
}

Vector3 CTransform::GetRelativeRot() const
{
	return m_vRelativeRot;
}

Vector3 CTransform::GetRelativePos() const
{
	return m_vRelativePos;
}

Vector3 CTransform::GetRelativeAxis(AXIS axis) const
{
	return m_vRelativeAxis[axis];
}

Vector3 CTransform::GetWorldScale() const
{
	return m_vWorldScale;
}

Vector3 CTransform::GetWorldRot() const
{
	return m_vWorldRot;
}

Vector3 CTransform::GetWorldPos() const
{
	return m_vWorldPos;
}

Vector3 CTransform::GetWorldPivot() const
{
	return m_vPivot;
}

Vector3 CTransform::GetWorldAxis(AXIS axis) const
{
	return m_vWorldAxis[axis];
}

void CTransform::SetWorldScale(const Vector3& v)
{
	m_vVelocityScale += v - m_vWorldScale;

	m_vWorldScale = v;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z) - m_vWorldScale;

	m_vWorldScale = Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::SetWorldRot(const Vector3& v)
{
	m_vVelocityRot += v - m_vWorldRot;

	m_vWorldRot = v;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent )
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}		

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z) - m_vWorldRot;

	m_vWorldRot = Vector3(x, y, z);

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldRotX(float x)
{
	m_vVelocityRot.x += x - m_vWorldRot.x;

	m_vWorldRot.x = x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldRotY(float y)
{
	m_vVelocityRot.y += y - m_vWorldRot.y;

	m_vWorldRot.y = y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldRotZ(float z)
{
	m_vVelocityRot.z += z - m_vWorldRot.z;

	m_vWorldRot.z = z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldPos(const Vector3& v)
{
	m_vVelocity += v - m_vWorldPos;

	m_vWorldPos = v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tMat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::SetWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z) - m_vWorldPos;

	m_vWorldPos = Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tMat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::AddWorldScale(const Vector3& v)
{
	m_vVelocityScale += v;

	m_vWorldScale += v;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::AddWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x,y,z);

	m_vWorldScale += Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::AddWorldRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vWorldRot += v;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z);

	m_vWorldRot += Vector3(x,y,z);

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddWorldRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vWorldRot.x = x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddWorldRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vWorldRot.y = y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddWorldRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vWorldRot.z = z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::AddWorldPos(const Vector3& v)
{
	m_vVelocity += v;

	m_vWorldPos += v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float)*3);

		tMat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::AddWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z);

	m_vWorldPos += Vector3(x,y,z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tMat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::SetPivot(const Vector3& v)
{
	m_vPivot = v;
}

void CTransform::SetPivot(float x, float y, float z)
{
	SetPivot(Vector3(x,y,z));
}

void CTransform::SetMeshSize(const Vector3& v)
{
	m_vMeshSize = v;
}
