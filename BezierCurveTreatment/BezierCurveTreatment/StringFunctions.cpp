#include "StringFunctions.h"


string StringFunctions::IntToString(int number_to_convert)
{
		string temp_str;
		stringstream out;
		out<<number_to_convert;
		out>>temp_str;
		return temp_str;
}

string StringFunctions::DoubleToString(double number_to_convert)
{
	string temp_str;
	stringstream out;
	out<<number_to_convert;
	out>>temp_str;
	return temp_str;
}

int StringFunctions::StringToInt(string str_to_convert)
{
	int temp_number;
	stringstream out;
	out<<str_to_convert;
	out>>temp_number;
	return temp_number;
}

double StringFunctions::StringToDouble(string str_to_convert)
{
	
		double temp_number;
		stringstream out;
		out<<str_to_convert;
		out>>temp_number;
		return temp_number;
	
}
