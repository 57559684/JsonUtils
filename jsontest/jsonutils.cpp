

#include <cstdio>
#include <string>
#include <ctype.h>
#include "jsonutils.h"


using namespace std;

//静态空值对象，用于构造空值的const CJobject对象
static Value gNullValue(kNullType);
static ValueW gNullValueW(kNullType);

/****************************************************
 * CJson的对象类
 ***************************************************/
CJsonUtils::CJsonUtils() : m_bRoot(true), m_pDoc(new Document()), m_pValue(new Value(kObjectType))
{

}

CJsonUtils::CJsonUtils(Document* pDoc, Value* pValue) : m_bRoot(false), m_pDoc(pDoc), m_pValue(pValue)
{
}

CJsonUtils::CJsonUtils(const CJsonUtils& other) : m_bRoot(false), m_pDoc(other.m_pDoc), m_pValue(other.m_pValue)
{
}

CJsonUtils::~CJsonUtils()
{
    if (true == m_bRoot)
    {
        delete m_pDoc;
        delete m_pValue;
    }
}

CJsonUtils& CJsonUtils::operator=(const CJsonUtils& Other)
{
    m_pValue->CopyFrom(*Other.m_pValue, m_pDoc->GetAllocator());
    return *this;
}

bool CJsonUtils::Parse(const char* strJsonText)
{
    if (false == m_bRoot)
        return false;

    m_pDoc->Parse(strJsonText);
    if (kParseErrorNone != m_pDoc->GetParseError())
        return false;

    *m_pValue = m_pDoc->Move();
    return true;
}

bool CJsonUtils::Parse(const string& strJsonText)
{
    return Parse(strJsonText.c_str());
}

string CJsonUtils::GetJsonText(bool isPretty) const
{
    StringBuffer stBuffer;
	if (isPretty)
	{
		PrettyWriter<StringBuffer> stWriter(stBuffer);
		m_pValue->Accept(stWriter);
	}
	else
	{
		Writer<StringBuffer> stWriter(stBuffer);
		m_pValue->Accept(stWriter);
	}


    return string(stBuffer.GetString(), stBuffer.GetSize());
}

bool CJsonUtils::IsObject() const
{
    switch (m_pValue->GetType())
    {
        case kStringType:
        case kNumberType:
        case kTrueType:
        case kFalseType:
            return false;

        default:
            return true;
    }
}

bool CJsonUtils::IsString() const
{
    return m_pValue->IsString();
}

bool CJsonUtils::IsNumber() const
{
    return m_pValue->IsNumber();
}

bool CJsonUtils::IsStringNumber() const
{
    if (false == IsString())
        return false;

    const char* cch = m_pValue->GetString();
    if ('-' == *cch)
        ++cch;

    int iDotCount = 0;
    for (; *cch != 0; ++cch)
    {
        if (0 == isdigit(*cch))
        {
            if ('.' != *cch)
                return false;

            ++iDotCount;
            if (1 < iDotCount)
                return false;
        }
    }

    return true;
}

bool CJsonUtils::IsInt32() const
{
    return m_pValue->IsInt();
}

bool CJsonUtils::IsInt64() const
{
    return m_pValue->IsInt64();
}

bool CJsonUtils::IsUint32() const
{
    return m_pValue->IsUint();
}

bool CJsonUtils::IsUint64() const
{
    return m_pValue->IsUint64();
}

bool CJsonUtils::IsDouble() const
{
    return m_pValue->IsDouble();
}

bool CJsonUtils::IsBool() const
{
	switch (m_pValue->GetType())
	{
	case kTrueType:
	case kFalseType:
		return true;
	}

	return m_pValue->IsBool();
}

bool CJsonUtils::IsExists(const char* strKey) const
{
    return m_pValue->HasMember(strKey);
}

bool CJsonUtils::IsExists(const std::string& strKey) const
{
    return IsExists(strKey.c_str());
}

bool CJsonUtils::Remove(const char* strKey) const
{
	if (m_pValue->HasMember(strKey))
	{
		return m_pValue->RemoveMember(strKey);
	}

	return false;
}

bool CJsonUtils::Remove(const std::string& strKey) const
{
	return Remove(strKey.c_str());
}

bool CJsonUtils::Remove(int32_t iArrayIndex) const
{
	if (false == m_pValue->IsArray())
		return false;

	if (m_pValue->Size() < iArrayIndex)
		return false;

	int index = 0;
	for (rapidjson::Value::ValueIterator iter = m_pValue->Begin(); iter != m_pValue->End();)
	{
		if (index == iArrayIndex)
		{
			m_pValue->Erase(iter);
			break;
		}
		++index;
		++iter;
	}

	return true;
}

CJsonUtils CJsonUtils::operator[](const char* strKey)
{
    Value::MemberIterator iter = m_pValue->FindMember(strKey);
    if (m_pValue->MemberEnd() == iter)
    {
        Value tKey(strKey, m_pDoc->GetAllocator());
        Value tValue(kObjectType);
        m_pValue->AddMember(tKey, tValue, m_pDoc->GetAllocator());

        iter = m_pValue->FindMember(strKey);
    }

    return CJsonUtils(m_pDoc, &iter->value);
}

CJsonUtils CJsonUtils::operator[](const string& strKey)
{
    Value::MemberIterator iter = m_pValue->FindMember(strKey.c_str());
    if (m_pValue->MemberEnd() == iter)
    {
        Value tKey(strKey.c_str(), strKey.length(), m_pDoc->GetAllocator());
        Value tValue(kObjectType);
        m_pValue->AddMember(tKey, tValue, m_pDoc->GetAllocator());

        iter = m_pValue->FindMember(strKey.c_str());
    }

    return CJsonUtils(m_pDoc, &iter->value);
}

const CJsonUtils CJsonUtils::operator[](const char* strKey) const
{
    Value::MemberIterator iter = m_pValue->FindMember(strKey);
    if (m_pValue->MemberEnd() == iter)
    {
        return CJsonUtils(m_pDoc, &gNullValue);
    }

    return CJsonUtils(m_pDoc, &iter->value);
}

const CJsonUtils CJsonUtils::operator[](const std::string& strKey) const
{
    return operator[](strKey.c_str());
}

int32_t CJsonUtils::ArrayCount() const
{
    if (false == m_pValue->IsArray())
        return -1;

    return m_pValue->Size();
}

CJsonUtils CJsonUtils::operator[](int32_t iArrayIndex)
{
    if (false == m_pValue->IsArray())
        m_pValue->SetArray();

    int32_t iSize = m_pValue->Size();
    for (int32_t i = iSize; i < iArrayIndex + 1; ++i)
    {
        Value tValue(kObjectType);
        m_pValue->PushBack(tValue, m_pDoc->GetAllocator());
    }

    Value& tValue = (*m_pValue)[iArrayIndex];
    return CJsonUtils(m_pDoc, &tValue);
}

const CJsonUtils CJsonUtils::operator[](int32_t iArrayIndex) const
{
    if (false == m_pValue->IsArray())
        return CJsonUtils(m_pDoc, &gNullValue);

    if ((SizeType)iArrayIndex >= m_pValue->Size())
        return CJsonUtils(m_pDoc, &gNullValue);

    Value& tValue = (*m_pValue)[iArrayIndex];
    return CJsonUtils(m_pDoc, &tValue);
}

CJsonUtils& CJsonUtils::operator=(const char* strValue)
{
    m_pValue->SetString(strValue, m_pDoc->GetAllocator());
    return *this;
}

CJsonUtils& CJsonUtils::operator=(const std::string& strValue)
{
    m_pValue->SetString(strValue.c_str(), strValue.length(), m_pDoc->GetAllocator());
    return *this;
}

CJsonUtils& CJsonUtils::operator=(int32_t iValue)
{
    m_pValue->SetInt(iValue);
    return *this;
}

CJsonUtils& CJsonUtils::operator=(int64_t i64Value)
{
    m_pValue->SetInt64(i64Value);
    return *this;
}

CJsonUtils& CJsonUtils::operator=(uint32_t uValue)
{
    m_pValue->SetUint(uValue);
    return *this;
}

CJsonUtils& CJsonUtils::operator=(uint64_t u64Value)
{
    m_pValue->SetUint64(u64Value);
    return *this;
}

CJsonUtils& CJsonUtils::operator=(double dValue)
{
    m_pValue->SetDouble(dValue);
    return *this;
}

CJsonUtils& CJsonUtils::operator=(bool bValue)
{
	m_pValue->SetBool(bValue);
	return *this;
}

CJsonUtils::operator string() const
{
    if (true == m_pValue->IsString())
    {
        return string(m_pValue->GetString(), m_pValue->GetStringLength());
    }
    else if (m_pValue->IsInt())
    {
        char szBuf[64] = {0};
        snprintf(szBuf, sizeof(szBuf), "%d", m_pValue->GetInt());
        return szBuf;
    }
    else if (m_pValue->IsInt64())
    {
        char szBuf[64] = {0};
        snprintf(szBuf, sizeof(szBuf), "%jd", m_pValue->GetInt64());
        return szBuf;
    }
    else if (m_pValue->IsUint())
    {
        char szBuf[64] = {0};
        snprintf(szBuf, sizeof(szBuf), "%u", m_pValue->GetUint());
        return szBuf;
    }
    else if (true == m_pValue->IsUint64())
    {
        char szBuf[64] = {0};
        snprintf(szBuf, sizeof(szBuf), "%ju", m_pValue->GetUint64());
        return szBuf;
    }
    else if (true == m_pValue->IsDouble())
    {
        char szBuf[64] = {0};
        snprintf(szBuf, sizeof(szBuf), "%g", m_pValue->GetDouble());
        return szBuf;
    }
    else
    {
        return GetJsonText();
    }
}

CJsonUtils::operator int32_t() const
{
    return ConvertToNumber<int32_t>(atoi, 0);
}

CJsonUtils::operator uint32_t() const
{
    return ConvertToNumber<uint32_t>(atoi, 0);
}

CJsonUtils::operator int64_t() const
{
    return ConvertToNumber<int64_t>(atoll, 0L);
}

CJsonUtils::operator uint64_t() const
{
    return ConvertToNumber<uint64_t>(atoll, 0L);
}

CJsonUtils::operator double() const
{
    return ConvertToNumber<double>(atof, 0.0);
}

CJsonUtils::operator bool() const
{
	return ConvertToNumber<bool>(atoi, 0);
}

template<typename _TYPE_, typename _CONVERT_FUNC_, typename _DEF_VAL>
_TYPE_ CJsonUtils::ConvertToNumber(_CONVERT_FUNC_ pFunc, _DEF_VAL tDefValue) const
{
    if (true == m_pValue->IsString())
    {
        return pFunc(m_pValue->GetString());
    }
    else if (true == m_pValue->IsInt())
    {
        return m_pValue->GetInt();
    }
    else if (true == m_pValue->IsInt64())
    {
        return m_pValue->GetInt64();
    }
    else if (true == m_pValue->IsUint())
    {
        return m_pValue->GetUint();
    }
    else if (true == m_pValue->IsUint64())
    {
        return m_pValue->GetUint64();
    }
    else if (true == m_pValue->IsDouble())
    {
        return m_pValue->GetDouble();
    }
	else if (true == m_pValue->IsBool())
	{
		return m_pValue->GetBool();
	}
    else
    {
        return tDefValue;
    }
}


CJsonUtilsW::CJsonUtilsW() : m_bRoot(true), m_pDoc(new DocumentW()), m_pValue(new ValueW(kObjectType))
{

}

CJsonUtilsW::CJsonUtilsW(DocumentW* pDoc, ValueW* pValue) : m_bRoot(false), m_pDoc(pDoc), m_pValue(pValue)
{
}

CJsonUtilsW::CJsonUtilsW(const CJsonUtilsW& other) : m_bRoot(false), m_pDoc(other.m_pDoc), m_pValue(other.m_pValue)
{
}

CJsonUtilsW::~CJsonUtilsW()
{
	if (true == m_bRoot)
	{
		delete m_pDoc;
		delete m_pValue;
	}
}

CJsonUtilsW& CJsonUtilsW::operator=(const CJsonUtilsW& Other)
{
	m_pValue->CopyFrom(*Other.m_pValue, m_pDoc->GetAllocator());
	return *this;
}

bool CJsonUtilsW::Parse(const wchar_t* strJsonText)
{
	if (false == m_bRoot)
		return false;

	m_pDoc->Parse(strJsonText);
	if (kParseErrorNone != m_pDoc->GetParseError())
		return false;

	*m_pValue = m_pDoc->Move();
	return true;
}

bool CJsonUtilsW::Parse(const wstring& strJsonText)
{
	return Parse(strJsonText.c_str());
}

wstring CJsonUtilsW::GetJsonText(bool isPretty) const
{
	StringBufferW stBuffer;
	if (isPretty)
	{
		PrettyWriter<StringBufferW, UTF16<>, UTF16<> > stWriter(stBuffer);
		m_pValue->Accept(stWriter);
	}
	else
	{
		Writer<StringBufferW, UTF16<>, UTF16<> > stWriter(stBuffer);
		m_pValue->Accept(stWriter);
	}


	return wstring(stBuffer.GetString(), stBuffer.GetSize());
}

bool CJsonUtilsW::IsObject() const
{
	switch (m_pValue->GetType())
	{
	case kStringType:
	case kNumberType:
	case kTrueType:
	case kFalseType:
		return false;

	default:
		return true;
	}
}

bool CJsonUtilsW::IsString() const
{
	return m_pValue->IsString();
}

bool CJsonUtilsW::IsNumber() const
{
	return m_pValue->IsNumber();
}

bool CJsonUtilsW::IsStringNumber() const
{
	if (false == IsString())
		return false;

	const wchar_t* cch = m_pValue->GetString();
	if (L'-' == *cch)
		++cch;

	int iDotCount = 0;
	for (; *cch != 0; ++cch)
	{
		if (0 == isdigit(*cch))
		{
			if ('.' != *cch)
				return false;

			++iDotCount;
			if (1 < iDotCount)
				return false;
		}
	}

	return true;
}

bool CJsonUtilsW::IsInt32() const
{
	return m_pValue->IsInt();
}

bool CJsonUtilsW::IsInt64() const
{
	return m_pValue->IsInt64();
}

bool CJsonUtilsW::IsUint32() const
{
	return m_pValue->IsUint();
}

bool CJsonUtilsW::IsUint64() const
{
	return m_pValue->IsUint64();
}

bool CJsonUtilsW::IsDouble() const
{
	return m_pValue->IsDouble();
}

bool CJsonUtilsW::IsBool() const
{
	switch (m_pValue->GetType())
	{
	case kTrueType:
	case kFalseType:
		return true;
	}

	return m_pValue->IsBool();
}

bool CJsonUtilsW::IsExists(const wchar_t* strKey) const
{
	return m_pValue->HasMember(strKey);
}

bool CJsonUtilsW::IsExists(const std::wstring& strKey) const
{
	return IsExists(strKey.c_str());
}

bool CJsonUtilsW::Remove(const wchar_t* strKey) const
{
	if (m_pValue->HasMember(strKey))
	{
		return m_pValue->RemoveMember(strKey);
	}

	return false;
}

bool CJsonUtilsW::Remove(const std::wstring& strKey) const
{
	return Remove(strKey.c_str());
}

bool CJsonUtilsW::Remove(int32_t iArrayIndex) const
{
	if (false == m_pValue->IsArray())
		return false;

	if (m_pValue->Size() < iArrayIndex)
		return false;

	int index = 0;
	for (ValueW::ValueIterator iter = m_pValue->Begin(); iter != m_pValue->End();)
	{
		if (index == iArrayIndex)
		{
			m_pValue->Erase(iter);
			break;
		}
		++index;
		++iter;
	}

	return true;
}

CJsonUtilsW CJsonUtilsW::operator[](const wchar_t* strKey)
{
	ValueW::MemberIterator iter = m_pValue->FindMember(strKey);
	if (m_pValue->MemberEnd() == iter)
	{
		ValueW tKey(strKey, m_pDoc->GetAllocator());
		ValueW tValue(kObjectType);
		m_pValue->AddMember(tKey, tValue, m_pDoc->GetAllocator());

		iter = m_pValue->FindMember(strKey);
	}

	return CJsonUtilsW(m_pDoc, &iter->value);
}

CJsonUtilsW CJsonUtilsW::operator[](const wstring& strKey)
{
	ValueW::MemberIterator iter = m_pValue->FindMember(strKey.c_str());
	if (m_pValue->MemberEnd() == iter)
	{
		ValueW tKey(strKey.c_str(), strKey.length(), m_pDoc->GetAllocator());
		ValueW tValue(kObjectType);
		m_pValue->AddMember(tKey, tValue, m_pDoc->GetAllocator());

		iter = m_pValue->FindMember(strKey.c_str());
	}

	return CJsonUtilsW(m_pDoc, &iter->value);
}

const CJsonUtilsW CJsonUtilsW::operator[](const wchar_t* strKey) const
{
	ValueW::MemberIterator iter = m_pValue->FindMember(strKey);
	if (m_pValue->MemberEnd() == iter)
	{
		return CJsonUtilsW(m_pDoc, &gNullValueW);
	}

	return CJsonUtilsW(m_pDoc, &iter->value);
}

const CJsonUtilsW CJsonUtilsW::operator[](const std::wstring& strKey) const
{
	return operator[](strKey.c_str());
}

int32_t CJsonUtilsW::ArrayCount() const
{
	if (false == m_pValue->IsArray())
		return -1;

	return m_pValue->Size();
}

CJsonUtilsW CJsonUtilsW::operator[](int32_t iArrayIndex)
{
	if (false == m_pValue->IsArray())
		m_pValue->SetArray();

	int32_t iSize = m_pValue->Size();
	for (int32_t i = iSize; i < iArrayIndex + 1; ++i)
	{
		ValueW tValue(kObjectType);
		m_pValue->PushBack(tValue, m_pDoc->GetAllocator());
	}

	ValueW& tValue = (*m_pValue)[iArrayIndex];
	return CJsonUtilsW(m_pDoc, &tValue);
}

const CJsonUtilsW CJsonUtilsW::operator[](int32_t iArrayIndex) const
{
	if (false == m_pValue->IsArray())
		return CJsonUtilsW(m_pDoc, &gNullValueW);

	if ((SizeType)iArrayIndex >= m_pValue->Size())
		return CJsonUtilsW(m_pDoc, &gNullValueW);

	ValueW& tValue = (*m_pValue)[iArrayIndex];
	return CJsonUtilsW(m_pDoc, &tValue);
}

CJsonUtilsW& CJsonUtilsW::operator=(const wchar_t* strValue)
{
	m_pValue->SetString(strValue, m_pDoc->GetAllocator());
	return *this;
}

CJsonUtilsW& CJsonUtilsW::operator=(const std::wstring& strValue)
{
	m_pValue->SetString(strValue.c_str(), strValue.length(), m_pDoc->GetAllocator());
	return *this;
}

CJsonUtilsW& CJsonUtilsW::operator=(int32_t iValue)
{
	m_pValue->SetInt(iValue);
	return *this;
}

CJsonUtilsW& CJsonUtilsW::operator=(int64_t i64Value)
{
	m_pValue->SetInt64(i64Value);
	return *this;
}

CJsonUtilsW& CJsonUtilsW::operator=(uint32_t uValue)
{
	m_pValue->SetUint(uValue);
	return *this;
}

CJsonUtilsW& CJsonUtilsW::operator=(uint64_t u64Value)
{
	m_pValue->SetUint64(u64Value);
	return *this;
}

CJsonUtilsW& CJsonUtilsW::operator=(double dValue)
{
	m_pValue->SetDouble(dValue);
	return *this;
}

CJsonUtilsW& CJsonUtilsW::operator=(bool bValue)
{
	m_pValue->SetBool(bValue);
	return *this;
}

CJsonUtilsW::operator wstring() const
{
	if (true == m_pValue->IsString())
	{
		return wstring(m_pValue->GetString(), m_pValue->GetStringLength());
	}
	else if (m_pValue->IsInt())
	{
		wchar_t szBuf[64] = { 0 };
		_snwprintf(szBuf, sizeof(szBuf), L"%d", m_pValue->GetInt());
		return szBuf;
	}
	else if (m_pValue->IsInt64())
	{
		wchar_t szBuf[64] = { 0 };
		_snwprintf(szBuf, sizeof(szBuf), L"%jd", m_pValue->GetInt64());
		return szBuf;
	}
	else if (m_pValue->IsUint())
	{
		wchar_t szBuf[64] = { 0 };
		_snwprintf(szBuf, sizeof(szBuf), L"%u", m_pValue->GetUint());
		return szBuf;
	}
	else if (true == m_pValue->IsUint64())
	{
		wchar_t szBuf[64] = { 0 };
		_snwprintf(szBuf, sizeof(szBuf), L"%ju", m_pValue->GetUint64());
		return szBuf;
	}
	else if (true == m_pValue->IsDouble())
	{
		wchar_t szBuf[64] = { 0 };
		_snwprintf(szBuf, sizeof(szBuf), L"%g", m_pValue->GetDouble());
		return szBuf;
	}
	else
	{
		return GetJsonText();
	}
}

CJsonUtilsW::operator int32_t() const
{
	return ConvertToNumber<int32_t>(_wtoi, 0);
}

CJsonUtilsW::operator uint32_t() const
{
	return ConvertToNumber<uint32_t>(_wtoi, 0);
}

CJsonUtilsW::operator int64_t() const
{
	return ConvertToNumber<int64_t>(_wtoll, 0L);
}

CJsonUtilsW::operator uint64_t() const
{
	return ConvertToNumber<uint64_t>(_wtoll, 0L);
}

CJsonUtilsW::operator double() const
{
	return ConvertToNumber<double>(_wtof, 0.0);
}

CJsonUtilsW::operator bool() const
{
	return ConvertToNumber<bool>(_wtoi, 0);
}

template<typename _TYPE_, typename _CONVERT_FUNC_, typename _DEF_VAL>
_TYPE_ CJsonUtilsW::ConvertToNumber(_CONVERT_FUNC_ pFunc, _DEF_VAL tDefValue) const
{
	if (true == m_pValue->IsString())
	{
		return pFunc(m_pValue->GetString());
	}
	else if (true == m_pValue->IsInt())
	{
		return m_pValue->GetInt();
	}
	else if (true == m_pValue->IsInt64())
	{
		return m_pValue->GetInt64();
	}
	else if (true == m_pValue->IsUint())
	{
		return m_pValue->GetUint();
	}
	else if (true == m_pValue->IsUint64())
	{
		return m_pValue->GetUint64();
	}
	else if (true == m_pValue->IsDouble())
	{
		return m_pValue->GetDouble();
	}
	else if (true == m_pValue->IsBool())
	{
		return m_pValue->GetBool();
	}
	else
	{
		return tDefValue;
	}
}