

#ifndef JSON_UTILS_H
#define	JSON_UTILS_H

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/encodings.h"


using namespace rapidjson;
//使用UTF-16的doc,在rapidjson命名空间里包含UTF8的Document和Value的定义,但是不包含UTF16的
typedef rapidjson::GenericDocument<rapidjson::UTF16<>> DocumentW;
typedef rapidjson::GenericValue<rapidjson::UTF16<>> ValueW;
typedef rapidjson::GenericStringStream<rapidjson::UTF16<>> StringStreamW;
typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<>> StringBufferW;


/**
 * Json对象类，封装Json操作的插入等方法
 * 使用范例如下：
 * CJsonUtils json;
 * json["a"] = "a";
 * json["b"] = "123";
 * json["c"] = 123;
 * json["d"] = "1.5";
 * json["e"] = 1.5
 * json["child"]["a"] = "1";
 * json["child"]["b"] = "2";
 * json["array"][0] = 1;
 * json["array"][1] = 2;
 *
 * std::string strJsonText = json.GetJsonText();
 *
 * CJsonUtils json2;
 * json2.Parse(strJsonText);
 *
 * std::string a1 = json2["a"];
 * int a2 = json2["a"];
 * double a3 = json2["a"];
 * int ca1 = json["child"]["a"];
 * int aa1 = json["array"][0];
 */
class CJsonUtils
{
private:
    CJsonUtils(rapidjson::Document* pDoc, rapidjson::Value* pValue);

public:
    CJsonUtils();
    CJsonUtils(const CJsonUtils& other);
    ~CJsonUtils();

    CJsonUtils& operator=(const CJsonUtils& Other);

    /**
     * 解析Json文本
     * @param strJsonText
     * @return
     */
    bool Parse(const char* strJsonText);
    bool Parse(const std::string& strJsonText);

    /**
     * 生成Json本文
     * @return Json文本字符串
     */
    std::string GetJsonText(bool isPretty = false) const;

public:
    /**
     * 判断当前存储的内容的什么类型
     * IsNumber = IsInt32|IsInt64|IsUint32|IsUint64|IsDouble
     *
     * @return
     *		true	是
     *		false	不是
     *
     * 注意:
     *		1.字符串数字，类型为字符串，而不是数字，因此请正确的存储值的类型
     *		2.对于整形而言，若值能正确转换为某种类型，则IsType=true.以下为举例:
     *			 1:		IsInt32、IsInt64、IsUint32、IsUint64 = true
     *			-1:		IsInt32、IsInt64   = true;
     *					IsUint32、IsUint64 = false
     *			50亿:	IsInt64、IsUint64  = true;
     *					IsInt32、 IsUint32 = false
     */
    bool IsObject() const;
    bool IsString() const;
    bool IsNumber() const;
    bool IsStringNumber() const;
    bool IsInt32() const;
    bool IsInt64() const;
    bool IsUint32() const;
    bool IsUint64() const;
    bool IsDouble() const;
	bool IsBool() const;

    /**
     * 判断是否存在制定的键值对
     * @param[in] strKey 键字符串
     * @return
     */
    bool IsExists(const char* strKey) const;
    bool IsExists(const std::string& strKey) const;


    /**
     * 删除指定键值对
     * @param[in] strKey 键字符串
     * @return
     */
    bool Remove(const char* strKey) const;
    bool Remove(const std::string& strKey) const;

    /**
     * 删除指定数组元素
     * @param[in] iArrayIndex 数组索引号
     * @return
     */
    bool Remove(int32_t iArrayIndex) const;

public:
    /**
     * 键值对操作，得到值对象
     * @param[in] strKey 键字符串
     * @return 值对象
     */
    CJsonUtils operator[](const char* strKey);
    CJsonUtils operator[](const std::string& strKey);
    const CJsonUtils operator[](const char* strKey) const;
    const CJsonUtils operator[](const std::string& strKey) const;

    /**
	 * 返回数组中元素的个数
	 * @return
	 *		>=0	元素个数
	 *		<0	不是数组
	 */
    int32_t ArrayCount() const;

    /**
     * 得到数组中指定索引的对象
     * @param[in] iArrayIndex 数组索引号
     * @return 值对象
     */
    CJsonUtils operator[](int32_t iArrayIndex);
    const CJsonUtils operator[](int32_t iArrayIndex) const;

public:
    /**
     * 赋值操作，修改对象存储的内容
     * @param[in] 各种Value
     * @return 对象自身的引用
     */
    CJsonUtils& operator=(const char* strValue);
    CJsonUtils& operator=(const std::string& strValue);
    CJsonUtils& operator=(int32_t iValue);
    CJsonUtils& operator=(int64_t i64Value);
    CJsonUtils& operator=(uint32_t uValue);
    CJsonUtils& operator=(uint64_t u64Value);
    CJsonUtils& operator=(double dValue);
	CJsonUtils& operator=(bool bValue);

    /**
     * 隐式转换，取值
     * @return
     *		std::string	若存储的是字符串，则的得到字符串
     *					若存储的是数值，则得到转换后的字符串
     *					若存储的是其他对象，则得到Json文本
     *
     *		int32_t、uint32_t、int64_t、uint64_t、double
     *					若存储的是数值，则返回数值本身
     *					若存储的是字符串形式的数值，则自动转换为对应的数值
     *					若存储的是其他类型，则返回0
     *
     * 注意:
     *		取值不会破坏原本的存储类型，例如:原本存储的 "1234" - 字符串1234，若用int来取值，
     *		得到的虽然是整形的1234，但是存储的内容还是字符串1234，不会改变
     */
    operator std::string() const;
    operator int32_t() const;
    operator uint32_t() const;
    operator int64_t() const;
    operator uint64_t() const;
    operator double() const;
	operator bool() const;

private:
    template<typename _TYPE_, typename _CONVERT_FUNC_, typename _DEF_VAL>
    _TYPE_ ConvertToNumber(_CONVERT_FUNC_ pFunc, _DEF_VAL tDefValue) const;

    bool                    m_bRoot;
    rapidjson::Document*    m_pDoc;
    rapidjson::Value*       m_pValue;
} ;

class CJsonUtilsW
{
private:
	CJsonUtilsW(DocumentW* pDoc, ValueW* pValue);

public:
	CJsonUtilsW();
	CJsonUtilsW(const CJsonUtilsW& other);
	~CJsonUtilsW();

	CJsonUtilsW& operator=(const CJsonUtilsW& Other);

	/**
	 * 解析Json文本
	 * @param strJsonText
	 * @return
	 */
	bool Parse(const wchar_t* strJsonText);
	bool Parse(const std::wstring& strJsonText);

	/**
	 * 生成Json本文
	 * @return Json文本字符串
	 */
	std::wstring GetJsonText(bool isPretty = false) const;

public:
	/**
	 * 判断当前存储的内容的什么类型
	 * IsNumber = IsInt32|IsInt64|IsUint32|IsUint64|IsDouble
	 *
	 * @return
	 *		true	是
	 *		false	不是
	 *
	 * 注意:
	 *		1.字符串数字，类型为字符串，而不是数字，因此请正确的存储值的类型
	 *		2.对于整形而言，若值能正确转换为某种类型，则IsType=true.以下为举例:
	 *			 1:		IsInt32、IsInt64、IsUint32、IsUint64 = true
	 *			-1:		IsInt32、IsInt64   = true;
	 *					IsUint32、IsUint64 = false
	 *			50亿:	IsInt64、IsUint64  = true;
	 *					IsInt32、 IsUint32 = false
	 */
	bool IsObject() const;
	bool IsString() const;
	bool IsNumber() const;
	bool IsStringNumber() const;
	bool IsInt32() const;
	bool IsInt64() const;
	bool IsUint32() const;
	bool IsUint64() const;
	bool IsDouble() const;
	bool IsBool() const;

	/**
	 * 判断是否存在制定的键值对
	 * @param[in] strKey 键字符串
	 * @return
	 */
	bool IsExists(const wchar_t* strKey) const;
	bool IsExists(const std::wstring& strKey) const;

	 /**
     * 删除指定键值对
     * @param[in] strKey 键字符串
     * @return
     */
    bool Remove(const wchar_t* strKey) const;
    bool Remove(const std::wstring& strKey) const;

	/**
	 * 删除指定数组元素
	 * @param[in] iArrayIndex 数组索引号
	 * @return
	 */
	bool Remove(int32_t iArrayIndex) const;

public:
	/**
	 * 键值对操作，得到值对象
	 * @param[in] strKey 键字符串
	 * @return 值对象
	 */
	CJsonUtilsW operator[](const wchar_t* strKey);
	CJsonUtilsW operator[](const std::wstring& strKey);
	const CJsonUtilsW operator[](const wchar_t* strKey) const;
	const CJsonUtilsW operator[](const std::wstring& strKey) const;

	/**
	 * 返回数组中元素的个数
	 * @return
	 *		>=0	元素个数
	 *		<0	不是数组
	 */
	int32_t ArrayCount() const;

	/**
	 * 得到数组中指定索引的对象
	 * @param[in] iArrayIndex 数组索引号
	 * @return 值对象
	 */
	CJsonUtilsW operator[](int32_t iArrayIndex);
	const CJsonUtilsW operator[](int32_t iArrayIndex) const;

public:
	/**
	 * 赋值操作，修改对象存储的内容
	 * @param[in] 各种Value
	 * @return 对象自身的引用
	 */
	CJsonUtilsW& operator=(const wchar_t* strValue);
	CJsonUtilsW& operator=(const std::wstring& strValue);
	CJsonUtilsW& operator=(int32_t iValue);
	CJsonUtilsW& operator=(int64_t i64Value);
	CJsonUtilsW& operator=(uint32_t uValue);
	CJsonUtilsW& operator=(uint64_t u64Value);
	CJsonUtilsW& operator=(double dValue);
	CJsonUtilsW& operator=(bool bValue);

	/**
	 * 隐式转换，取值
	 * @return
	 *		std::string	若存储的是字符串，则的得到字符串
	 *					若存储的是数值，则得到转换后的字符串
	 *					若存储的是其他对象，则得到Json文本
	 *
	 *		int32_t、uint32_t、int64_t、uint64_t、double
	 *					若存储的是数值，则返回数值本身
	 *					若存储的是字符串形式的数值，则自动转换为对应的数值
	 *					若存储的是其他类型，则返回0
	 *
	 * 注意:
	 *		取值不会破坏原本的存储类型，例如:原本存储的 "1234" - 字符串1234，若用int来取值，
	 *		得到的虽然是整形的1234，但是存储的内容还是字符串1234，不会改变
	 */
	operator std::wstring() const;
	operator int32_t() const;
	operator uint32_t() const;
	operator int64_t() const;
	operator uint64_t() const;
	operator double() const;
	operator bool() const;

private:
	template<typename _TYPE_, typename _CONVERT_FUNC_, typename _DEF_VAL>
	_TYPE_ ConvertToNumber(_CONVERT_FUNC_ pFunc, _DEF_VAL tDefValue) const;

	bool                    m_bRoot;
	DocumentW*				m_pDoc;
	ValueW*					m_pValue;
};


#endif //JSON_UTILS_H