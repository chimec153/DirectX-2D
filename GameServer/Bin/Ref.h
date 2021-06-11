#pragma once

#include "Server.h"
namespace Server
{
	class CRef
	{
	protected:
		CRef();
		CRef(const CRef& ref);
		virtual ~CRef();

	private:
		int	m_iRef;
		std::string	m_strKey;

	public:
		const std::string& GetKey()	const;
		void SetKey(const std::string& strKey);
		void AddRef();
		int Release();

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);
	};

}