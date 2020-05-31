#ifndef _STRING_FUNCTIONS
#define _STRING_FUNCTIONS
#include <string>
#include <sstream>

using namespace std;
class StringFunctions
{
	//Outdated class that been used for debugging program. Now program is not anymore need this class
public:
	static string IntToString(int number_to_convert);
	static string DoubleToString(double number_to_convert);
	static int StringToInt(string str_to_convert);
	static double StringToDouble(string str_to_convert);
};
#endif