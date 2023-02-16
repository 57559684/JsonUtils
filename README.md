# JsonUtils

```
CJsonUtils Class,Win32

Class name: CJsonUtils, CJsonUtilsW

Based on https://github.com/dongbingliu/CPlusPlusSample

Features:
Win Platform RapidJson wrapper
Support json parsing and packaging in utf8 and utf16 formats


Author: 57559684 2023-1-10
If you have problems using this log library, you can send an email to <57559684@qq.com>

功能介绍：
Win平台RapidJson封装
支持utf8和utf16格式的json解析和打包


作者：57559684 2023-1-10
如果您在使用本json库时遇到问题，可以发邮件至<57559684@qq.com>

用法：
1：在项目中添加logger.h和logger.cpp
2：设置logger.cpp的预编译头选项为“不使用预编译头”
3：使用代码示例：

Usage:
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
	cJsonUtils0["child"]["a"] = "child a parm";
	cJsonUtils0["child"]["b"] = "child b parm";
	cJsonUtils0["array"][0] = cJsonUtils1;
	cJsonUtils0["array"][1] = cJsonUtils2;

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

	string strJsonText2 = cJsonUtilsd.GetJsonText(true);

	CJsonUtilsW cJsonUtilsw1;

	cJsonUtilsw1[L"b"] = L"123";
	cJsonUtilsw1[L"c"] = L"1234";

	CJsonUtilsW cJsonUtilsw;
	cJsonUtilsw.Parse(wdatatt);

	cJsonUtilsw[L"test"] = cJsonUtilsw1;

	wstring strJsonText3 = cJsonUtilsw.GetJsonText(true);

```

