
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "jsonutils.h"


using namespace std;

string datatt =
"{\"Int\":1,"
"\"Double\":12.0000001,"
"\"String\":\"This is a string\","
"\"Object\":{\"name\":\"qq849635649\",\"age\":25},"
"\"IntArray\":[10,20,30],"
"\"DoubleArray\":[1.0,2.0,3.0],"
"\"StringArray\":[\"one\",\"two\",\"three\"],"
"\"MixedArray\":[\"one\",50,false,12.005],"
"\"People\":[{\"name\":\"qq849635649\",\"age\":0,\"sex\":true},"
"{\"name\":\"qq849635649\",\"age\":10,\"sex\":false},"
"{\"name\":\"qq849635649\",\"age\":20,\"sex\":true}]}";


wstring wdatatt =
L"{\"Int\":1,"
L"\"Double\":12.0000001,"
L"\"String\":\"This is a string\","
L"\"Object\":{\"name\":\"qq849635649\",\"age\":25},"
L"\"IntArray\":[10,20,30],"
L"\"DoubleArray\":[1.0,2.0,3.0],"
L"\"StringArray\":[\"one\",\"two\",\"three\"],"
L"\"MixedArray\":[\"one\",50,false,12.005],"
L"\"People\":[{\"name\":\"qq849635649\",\"age\":0,\"sex\":true},"
L"{\"name\":\"qq849635649\",\"age\":10,\"sex\":false},"
L"{\"name\":\"qq849635649\",\"age\":20,\"sex\":true}]}";

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Hello, World!" << std::endl;
	CJsonUtils cJsonUtils0;
	CJsonUtils cJsonUtils1;
	CJsonUtils cJsonUtils2;

	cJsonUtils1["b"] = "123";
	cJsonUtils1["c"] = "1234";

	cJsonUtils2["d"] = "1235";
	cJsonUtils2["e"] = "1236";


	cJsonUtils0["a"] = "dongbingliu";
	cJsonUtils0["b"] = "123";
	cJsonUtils0["c"] = "1234";
	cJsonUtils0["d"] = "1235";
	cJsonUtils0["e"] = "1236";
	cJsonUtils0["f"] = false;
	cJsonUtils0["g"] = true;
	cJsonUtils0["child"]["a"] = "child a parm";
	cJsonUtils0["child"]["b"] = "child b parm";
	cJsonUtils0["array"][0] = cJsonUtils1;
	cJsonUtils0["array"][1] = cJsonUtils2;

	cJsonUtils0.Remove("f");

	string strJsonText = cJsonUtils0.GetJsonText();

	cout << strJsonText.c_str() << endl;

	CJsonUtils cJsonUtils;
	cJsonUtils.Parse(strJsonText);
	string a1 = cJsonUtils["a"];
	cout << a1.c_str() << endl;

	cJsonUtils["test"] = cJsonUtils1;

	cJsonUtils["child"]["b"] = "asdf";
	string strJsonText1 = cJsonUtils.GetJsonText();


	string a2 = cJsonUtils["child"]["b"];
	cout << a2.c_str() << endl;


	int a3 = cJsonUtils["array"][1];
	cout << a3 << endl;

	CJsonUtils cJsonUtilsd;
	cJsonUtilsd.Parse(datatt);

	cJsonUtilsd["test"] = cJsonUtils1;
	cJsonUtilsd["Object"].Remove("age");
	cJsonUtilsd["IntArray"].Remove(0);

	string strJsonText2 = cJsonUtilsd.GetJsonText(true);

	CJsonUtilsW cJsonUtilsw1;

	cJsonUtilsw1[L"b"] = L"123";
	cJsonUtilsw1[L"c"] = L"1234";

	CJsonUtilsW cJsonUtilsw;
	cJsonUtilsw.Parse(wdatatt);

	cJsonUtilsw[L"test"] = cJsonUtilsw1;

	wstring strJsonText3 = cJsonUtilsw.GetJsonText(true);

	std::cout << "Hello, World!" << std::endl;

	string data;
	ifstream in("2.txt");
	if (!in.is_open())
	{
		cout << "Error while opening file.";
		return 1;
	}

	ostringstream sin;
	sin << in.rdbuf();
	data = sin.str();
	in.close();
	in.clear();

	CJsonUtils cJsonUtils3;
	cJsonUtils3.Parse(data);

	string strJsonText4 = cJsonUtils3.GetJsonText(true);

	string strJsonText5 = cJsonUtils3["result"];
	string strJsonText6 = cJsonUtils3["result"]["SyncDiskFileInfoRsp"];
	string strJsonText7 = cJsonUtils3["result"]["SyncDiskFileInfoRsp"]["contentInfoList"];
	string strJsonText8 = cJsonUtils3["result"]["SyncDiskFileInfoRsp"]["contentInfoList"][0];
	uint64_t strJsonText9 = cJsonUtils3["result"]["SyncDiskFileInfoRsp"]["contentInfoList"][0]["fileVersion"];
	bool exist = cJsonUtils3["result"]["SyncDiskFileInfoRsp"]["contentInfoList"][0].IsExists("fileVersion");


	return 0;
}