#include "stdafx.h"
#include "String.h"

wstring String::ToWString(string value)
{
	wstring temp = L"";
	temp.assign(value.begin(), value.end());
	// assign�� value�� ��� �����ؼ� temp�� �����Ѵ�

	return temp;
}

string String::ToString(wstring value)
{
	string temp = "";
	temp.assign(value.begin(), value.end());

	return temp;
}
