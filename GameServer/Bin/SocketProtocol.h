#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <string>
#include <mutex>
#include "Server.h"

namespace Server
{
#define MAX_CONTENTS_SIZE      512
#define MAX_MESSAGE_SIZE      1024
#define MAX_MSG_HEADER_SIZE      2 
#define MAX_MSG_CONTENTS_SIZE   3
#define MAX_MSG_TIME_SIZE      20
#define MAX_MSG_ID_SIZE         32

	enum class MessageHeader
	{
		None,
		Echo,
		ACK,
		NCK,
		Time,
		GetTime,
		DiffTime,
		Chat,
	};

	typedef struct _tagMessageStruct
	{
		char         message[MAX_MESSAGE_SIZE];
		uint32_t      messageSize;
		MessageHeader   eHeader;
		uint64_t      iTime;
		std::string      id;
		char         contents[MAX_CONTENTS_SIZE];
		uint32_t      contentsSize;

		_tagMessageStruct()
			: messageSize(0), contentsSize(0), iTime(0), eHeader(MessageHeader::None) {
		}
	}MessageStruct;

	typedef union {
		unsigned long l;
		float f;
	}u;

	class CSocketProtocol
	{
		static const char STX[2];
		static const char ETX[2];

		static int64_t s_iDiffTime;
		static std::string s_strID;
		static uint64_t		s_iTime;
		static std::mutex	s_hTimeMutex;

	public:
		template <typename T>
		static void TranslateNumberToMessage(char* msg, uint32_t& index, const T& iSrc)
		{
			uint8_t size = sizeof(iSrc);
			T data = iSrc;

			for (uint8_t i = 0; i < size; ++i)
			{
				byte currByte = data & 0xFF;
				data >>= 8;
				msg[index++] = currByte;
			}
		}

		template <typename T>
		static void TranslateNumberToMessage(char* msg, const uint32_t& index, const T& iSrc)
		{
			uint8_t size = sizeof(iSrc);
			T data = iSrc;

			for (uint8_t i = 0; i < size; ++i)
			{
				byte currByte = data & 0xFF;
				data >>= 8;
				msg[index + i] = currByte;
			}
		}

		template <typename T>
		static void TranslateMessageToNumber(T& dest, const char* msg, uint32_t& index)
		{
			dest = 0;

			uint8_t size = sizeof(T);

			for (uint8_t i = 0; i < size; ++i)
			{
				dest += (static_cast<T>(msg[index++] & 0xFF) << (i * 8));
			}
		}

		template <typename T>
		static void TranslateMessageToNumber(T& dest, const char* msg, const uint32_t& index)
		{
			dest = 0;

			uint8_t size = sizeof(T);

			for (uint8_t i = 0; i < size; ++i)
			{
				dest += (static_cast<T>(msg[index + i] & 0xFF) << (i * 8));
			}
		}

		static void TranslateMessageToID(std::string& strID, const char* msg, uint32_t& index)
		{
			for (int i = 0; i < MAX_MSG_ID_SIZE; ++i)
			{
				if (NULL != msg[index])
				{
					strID += msg[index];
				}

				++index;
			}
		}

		static void TranslateMessageToID(std::string& strID, const char* msg, const uint32_t& index)
		{
			for (int i = 0; i < MAX_MSG_ID_SIZE; ++i)
			{
				if (NULL != msg[index + i])
				{
					strID += msg[index + i];
				}
			}
		}

		static void TranslateIDToMessage(char* msg, uint32_t& index, const std::string& strID)
		{
			for (int i = 0; i < MAX_MSG_ID_SIZE; ++i)
			{
				if (strID.length() > i)
				{
					msg[index++] = strID[i];
				}
				else
				{
					msg[index++] = NULL;
				}
			}
		}

		static void TranslateStringToMessage(char* msg, uint32_t& index, const std::string& str)
		{
			uint32_t iSize = str.size();

			TranslateNumberToMessage<uint32_t>(msg, index, iSize);

			for (uint32_t i = 0; i < iSize; ++i)
			{
				msg[index++] = str[i];
			}
		}

		static void TranslateMessageToString(std::string& str, const char* msg, uint32_t& index)
		{
			uint32_t iSize;

			TranslateMessageToNumber<uint32_t>(iSize, msg, index);

			for (uint32_t i = 0; i < iSize; ++i)
			{
				str += msg[index++];
			}
		}

		static void TranslateMessageToString(std::string& str, const char* msg, const uint32_t& index)
		{
			uint32_t iSize;

			TranslateMessageToNumber<uint32_t>(iSize, msg, index);

			for (uint32_t i = 0; i < iSize; ++i)
			{
				str += msg[index + i];
			}
		}

		static void TranslateMessageToMessageHeader(MessageHeader& eHeader, const char* msg, const uint32_t& index)
		{
			uint32_t iHeader;
			TranslateMessageToNumber<uint32_t>(iHeader, msg, index);

			eHeader = static_cast<MessageHeader>(iHeader);
		}

		static void TranslateMessageToMessageHeader(MessageHeader& eHeader, const char* msg, uint32_t& index)
		{
			uint32_t iHeader;
			TranslateMessageToNumber<uint32_t>(iHeader, msg, index);

			eHeader = static_cast<MessageHeader>(iHeader);
		}

		static void TranslateMessageHeaderToMessage(char* msg, uint32_t& index, const MessageHeader& eHeader)
		{
			TranslateNumberToMessage<uint32_t>(msg, index, static_cast<uint32_t>(eHeader));
		}

	public:

		static void TranslateFloatToMessage(char* msg, uint32_t& index, float src)
		{
			u _src;

			_src.f = src;

			TranslateNumberToMessage<unsigned long>(msg, index, _src.l);
		}

		//Vector
		static void TranslateVectorToMessage(char* msg, uint32_t& index, float x, float y)
		{
			u _x, _y;

			_x.f = x;
			_y.f = y;

			TranslateNumberToMessage<unsigned long>(msg, index, _x.l);
			TranslateNumberToMessage<unsigned long>(msg, index, _y.l);
		}

		static void TranslateVectorToMessage(char* msg, uint32_t& index, float x, float y, float z)
		{
			u _x, _y, _z;

			_x.f = x;
			_y.f = y;
			_z.f = z;

			TranslateNumberToMessage<unsigned long>(msg, index, _x.l);
			TranslateNumberToMessage<unsigned long>(msg, index, _y.l);
			TranslateNumberToMessage<unsigned long>(msg, index, _z.l);
		}

		static void TranslateMessageToFloat(float& dest, const char* msg, uint32_t& index)
		{
			u _dest;

			TranslateMessageToNumber<unsigned long>(_dest.l, msg, index);

			dest = _dest.f;
		}

		static void TranslateMessageToVector(float& x, float& y, const char* msg, uint32_t& index)
		{
			u _x, _y;

			TranslateMessageToNumber<unsigned long>(_x.l, msg, index);
			TranslateMessageToNumber<unsigned long>(_y.l, msg, index);

			x = _x.f;
			y = _y.f;
		}

		static void TranslateMessageToVector(float& x, float& y, float& z, const char* msg, uint32_t& index)
		{
			u _x, _y, _z;

			TranslateMessageToNumber<unsigned long>(_x.l, msg, index);
			TranslateMessageToNumber<unsigned long>(_y.l, msg, index);
			TranslateMessageToNumber<unsigned long>(_z.l, msg, index);

			x = _x.f;
			y = _y.f;
			z = _z.f;
		}

		static void TranslateMessageToFloat(float& dest, const char* msg, const uint32_t& index)
		{
			u _dest;

			TranslateMessageToNumber<unsigned long>(_dest.l, msg, index);

			dest = _dest.f;
		}

		static void TranslateMessageToVector(float& x, float& y, const char* msg, const uint32_t& index)
		{
			u _x, _y;

			TranslateMessageToNumber<unsigned long>(_x.l, msg, index);
			TranslateMessageToNumber<unsigned long>(_y.l, msg, index);

			x = _x.f;
			y = _y.f;
		}

		static void TranslateMessageToVector(float& x, float& y, float& z, const char* msg, const uint32_t& index)
		{
			u _x, _y, _z;

			TranslateMessageToNumber<unsigned long>(_x.l, msg, index);
			TranslateMessageToNumber<unsigned long>(_y.l, msg, index);
			TranslateMessageToNumber<unsigned long>(_z.l, msg, index);

			x = _x.f;
			y = _y.f;
			z = _z.f;
		}

	public:
		static void SetDiffTime(int64_t iDiffTime);
		static int64_t GetDiffTime();
		static void AddDiffTime(int32_t iDiffTime);
		static void SetID(const std::string& strName);
		static std::string GetID();
		static void SetTime(const uint64_t& time);
		static uint64_t GetTime();
		static uint64_t GetServerTime();

	public:
		static uint64_t TranslateTimeToServerTime(uint64_t time);
		static uint64_t TranslateServerTimeToTime(uint64_t time);

	public:
		static void InitMessageStructWithTime(MessageStruct& msgStruct, MessageHeader eHeader);
		static void InitMessageStruct(MessageStruct& msgStruct, MessageHeader eHeader);
		static void InitMessageStructWithACK(MessageStruct& msgStruct, MessageHeader eHeader);
		static void InitMessageStructWithNCK(MessageStruct& msgStruct, MessageHeader eHeader);
		static void MakeMessageStructForACK(MessageStruct& msgStruct, const MessageStruct* pRecvMsgStruct);
		static void MakeMessageStructForNCK(MessageStruct& msgStruct, const MessageStruct* pRecvMsgStruct);
		static void MakeMessageStructForACK(MessageStruct& msgStruct, const MessageHeader eHeader);
		static void MakeMessageStructForNCK(MessageStruct& msgStruct, const MessageHeader eHeader);
		static void MakeMessageStructFromContents(MessageStruct& msgStruct);
		static void MakeMessageStructFromContentsWithoutTimeSync(MessageStruct& msgStruct);
		static uint32_t MakeMessageStructFromPacket(const char* packet, uint32_t size, std::vector<MessageStruct*>* pVecMsgStruct);

	public:
		static void MakeMessageStructAboutTime(MessageStruct& msgStruct, const uint64_t& clientTime);
		static void MakeMessageStructAboutGetTime(MessageStruct& msgStruct);
		static void MakeMessageStructAboutDiffTime(MessageStruct& msgStruct, const int64_t& diffTime);
		static void MakeMessageStructAboutChat(MessageStruct& msgStruct, const char* text);
		static void MakeMessageStructAboutChat(MessageStruct& msgStruct, std::string& text);

	};
}