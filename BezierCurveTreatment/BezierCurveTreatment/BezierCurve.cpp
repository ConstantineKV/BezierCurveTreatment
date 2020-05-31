#include "BezierCurve.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <cctype>
#include <string>

using namespace std;
//Constructors
BezierCurve::BezierCurve()
{
	Curve_Ready = false;
	Name = "";
	Argument = "";
	CurveBody = "";
	NumberOfPoints = 0;
}

BezierCurve::BezierCurve(string file_path, string func_name, string arg_name, int& error_b)
{
	string line,temp_name, temp_argument;
	int bezier_error = 0;

	fstream* text_file = new fstream(file_path.c_str(),ios::in);
	text_file->seekg(0,ios::beg);
	text_file->clear();
	
	try 
	{
		if (!text_file->is_open())
			throw 1000;

		while(!text_file->eof())
			{
				*text_file>>line;
				temp_name = NameFromDescription(line);
				temp_argument = ArgumentFromDescription(line);
				
				if ((temp_name==func_name) && (temp_argument==arg_name))
				{
					if (!CheckNameOrArgument(temp_name))
						throw 1001;
					if (!CheckNameOrArgument(temp_argument))
						throw 1002;
					if (!CheckDescription(line))
						throw 1003;
					string Dot_string;
					point_b Dot;
					Name = temp_name;
					Argument = temp_argument;
					CurveBody = BodyFromDescription(line);
					NumberOfPoints = NumberOfPointsFromDescription(CurveBody);
					for (int i=0;i<NumberOfPoints;i++)
					{
						*text_file>>Dot_string;
						if(!CheckPointString(Dot_string))
							throw 1005;
						Dot=PointFromDescription(Dot_string);
						Points.push_back(Dot);
					}
					if (Points.size()!=NumberOfPoints)
						throw 1006;
					Curve_Ready=true;
					break;
				}
				else
				{
					char symbol=NULL;
					while((symbol!='\n')&&(!text_file->eof()))
						text_file->get(symbol);
					if (text_file->eof())
						throw 1010;
				}
		}	
	}
	
	catch (int b_error)
	{
				bezier_error=b_error;
				Name = "";
				Argument = "";
				CurveBody = "";
				NumberOfPoints = 0;
				Points.clear();
				Curve_Ready = false;
	}

	if (text_file->is_open())
		text_file->close();
	delete text_file;
	error_b = bezier_error;
	
}
BezierCurve::BezierCurve(string new_description, vector<point_b> new_points, int& error_b)
{
	int bezier_error = 0;
	try
	{
		if (!CheckDescription(new_description))
			throw 1003;

		NumberOfPoints = NumberOfPointsFromDescription(new_description);
		if (NumberOfPoints!=new_points.size())
			throw 1006;
		Name = NameFromDescription(new_description);
		Argument = ArgumentFromDescription(new_description);
		CurveBody = BodyFromDescription(new_description);
		Points = new_points;
		Curve_Ready = true;
	}
	catch (int b_error)
	{
		bezier_error=b_error;
		Name = "";
		Argument = "";
		CurveBody = "";
		NumberOfPoints = 0;
		Points.clear();
		Curve_Ready = false;
	}

	error_b = bezier_error;
}


//Internal Calculation methods
unsigned int BezierCurve::factorial(unsigned int n)
{
	if (n == 0)
		return 1;
	if (n == 1)
		return 1;
	else
		return n * factorial(n-1);
}
unsigned int BezierCurve::binome(int i, int n)
{
	return factorial(n)/(factorial(i)*factorial(n-i));
}
double BezierCurve::polinome(int i, int n, double t)
{
	unsigned int Binome = binome(i,n);
	return Binome*pow(t,i)*pow(1-t,n-i);
}

grad_b BezierCurve::GradientOfDistance(BezierCurve* SecondCurve, double FirstArg, double SecondArg, unsigned short accuracy)
{
	grad_b gradient_b = {0,0,0};
	if ((this->Is_Ready())&&(SecondCurve->Is_Ready()))
	{
		double step = pow(0.1,accuracy);
		double first_d, second_d;
		//Calculating gradient for first argument
		if((FirstArg+step<=1)&&(FirstArg-step>=0))
		{
			first_d = DistancePointsCurves(SecondCurve,FirstArg+step,SecondArg);
			second_d = DistancePointsCurves(SecondCurve,FirstArg-step,SecondArg);
			gradient_b.first_grad=(first_d - second_d)/(2*step);
		}
		if(FirstArg+step>1)
		{

			first_d = DistancePointsCurves(SecondCurve,FirstArg,SecondArg);
			second_d = DistancePointsCurves(SecondCurve,FirstArg-step,SecondArg);
			gradient_b.first_grad=(first_d-second_d)/step;
		}
		if(FirstArg-step<0)
		{
			first_d = DistancePointsCurves(SecondCurve,FirstArg+step,SecondArg);
			second_d = DistancePointsCurves(SecondCurve,FirstArg,SecondArg);
			gradient_b.first_grad=(first_d-second_d)/step;
		}
		//Calculating gradient for second argument
		if((SecondArg+step<=1)&&(SecondArg-step>=0))
		{
			first_d = DistancePointsCurves(SecondCurve,FirstArg,SecondArg+step);
			second_d = DistancePointsCurves(SecondCurve,FirstArg,SecondArg-step); 
			gradient_b.second_grad=(first_d-second_d)/(2*step);
		}
		if(SecondArg+step>1)
		{
			first_d = DistancePointsCurves(SecondCurve,FirstArg,SecondArg);
			second_d = DistancePointsCurves(SecondCurve,FirstArg,SecondArg-step); 
			gradient_b.second_grad=(first_d-second_d)/step;
		}
		if(SecondArg-step<0)
		{
			first_d = DistancePointsCurves(SecondCurve,FirstArg,SecondArg+step);
			second_d = DistancePointsCurves(SecondCurve,FirstArg,SecondArg);
			gradient_b.second_grad=(first_d-second_d)/step;
		}
		//Calculating full gradient
		gradient_b.full_grad = sqrt(pow(gradient_b.first_grad,2)+pow(gradient_b.second_grad,2));
		return gradient_b;
	}
	else
		return gradient_b;
}



//Internal Service methods
string BezierCurve::IntToString(int number_to_convert)
{
	string temp_str;
	stringstream out;
	out<<number_to_convert;
	out>>temp_str;
	return temp_str;
}
string BezierCurve::DoubleToString(double number_to_convert)
{
	string temp_str;
	stringstream out;
	out<<number_to_convert;
	out>>temp_str;
	return temp_str;
}

int BezierCurve::StringToInt(string str_to_convert)
{
	int temp_number;
	stringstream out;
	out<<str_to_convert;
	out>>temp_number;
	return temp_number;
}

double BezierCurve::StringToDouble(string str_to_convert)
{

	double temp_number;
	stringstream out;
	out<<str_to_convert;
	out>>temp_number;
	return temp_number;

}

//Status methods
bool BezierCurve::Is_Ready()
{
	return Curve_Ready;
}

string BezierCurve::GetFunctionName()
{
	return Name;
}
string BezierCurve::GetFunctionArgument()
{
	return Argument;
}
string BezierCurve::GetFunctionNameAndArgument()
{
	if ((Name=="") || (Argument==""))
		return "";
	string NameAndArg = Name;
	NameAndArg.append("(");
	NameAndArg.append(Argument);
	NameAndArg.append(")");
	return NameAndArg;
}
string BezierCurve::GetFunctionBody()
{
	return CurveBody;
}

//Check methods
bool BezierCurve::CheckNameOrArgument(string name_or_argument)
{
	for(unsigned int i=0;i<name_or_argument.length();i++)
	{
		if (!isalpha(name_or_argument[i]))
			return false;
	}
	return true;
}

bool BezierCurve::CheckNumberString(string number_string)
{
	for(unsigned int i=0;i<number_string.length();i++)
	{
		if ((!isdigit(number_string[i]))&&(number_string[i]!='.')&&(number_string[i]!='-'))
			return false;
	}
	return true;
}
bool BezierCurve::CheckBody(string body, string argument, unsigned int number_of_points)
{
	vector<string> summands;
	unsigned int pos1=0, pos2=0, position=0;
	string temp_str, compare_str;
	if (number_of_points<2)
		return false;
	while((pos1<=body.size())&&(pos1!=-1))
	{	
		pos2=body.find('+',pos1+1);
		if (pos2==-1)
			pos2=body.size();
		temp_str = body.substr(pos1,pos2-pos1);

		summands.push_back(temp_str);
		pos1=pos2+1;
	}

	if ((summands.size()==0)||(summands.size()!=number_of_points)||(summands.size()<2))
		return false;

	for(unsigned int i=0;i<summands.size();i++)
	{
		if(i==0)												//first summand. Example: (1-t)^3
		{
			//check round brackets expression (t-1)^n
			pos1 = 0;
			pos1 = summands[i].find('*',pos1);
			temp_str = summands[i].substr(0,pos1);
			if (number_of_points<=2)
			{
				compare_str = "(1-";
				compare_str.append(argument);
				compare_str.append(")");
			}	
			else
			{
				compare_str = "(1-";
				compare_str.append(argument);
				compare_str.append(")^");
				compare_str.append(IntToString(number_of_points-1));
			}

			if ((temp_str!=compare_str)||(pos1==-1))
				return false;
			//check dot expression Pi
			pos1++;
			pos2 = summands[i].size();
			temp_str = summands[i].substr(pos1,pos2-pos1);
			compare_str = "P";
			compare_str.append(IntToString(i));
			if ((temp_str!=compare_str)||(pos2==-1))
				return false;
		}

		if((i>0)&&(i<number_of_points-1))														//inner summand. Example: 3*t*(1-t)^2*P1 or 3*t^2*(1-t)*P2
		{
			//check argument expression t^i
			pos1 = 0;
			pos2 = 0;
			pos1 = summands[i].find('*',pos1);
			temp_str = summands[i].substr(0,pos1);
			compare_str = IntToString(binome(i,number_of_points-1));
			if ((temp_str!=compare_str)||(pos1==-1))
				return false;
			//check argument t
			pos1++;	
			pos2 = summands[i].find('*',pos1);
			temp_str = summands[i].substr(pos1,pos2-pos1);
			if (i>1)
			{
				compare_str = argument;
				compare_str.append("^");
				compare_str.append(IntToString(i));
			}
			else
				compare_str = argument;
			if ((temp_str!=compare_str)||(pos2==-1))
				return false;
			pos1=pos2;
			//check round brackets expression (t-1)^n
			pos1++;
			pos2 = summands[i].find('*',pos1);
			temp_str = summands[i].substr(pos1,pos2-pos1);
			if (number_of_points-1-i<2)
			{
				compare_str = "(1-";
				compare_str.append(argument);
				compare_str.append(")");
			}
			else
			{
				compare_str = "(1-";
				compare_str.append(argument);
				compare_str.append(")^");
				compare_str.append(IntToString(number_of_points-1-i));
			}
			if ((temp_str!=compare_str)||(pos2==-1))
				return false;
			pos1=pos2;
			//check dot expression Pn
			pos1++;
			pos2 = summands[i].size();
			temp_str = summands[i].substr(pos1,pos2-pos1);
			compare_str="P";
			compare_str.append(IntToString(i));
			if ((temp_str!=compare_str)||(pos2==-1))
				return false;
		}
		if (i==number_of_points-1)																	//last summand. Example: t^3*P3
		{

			//check argument expression t^i
			pos1 = 0;
			pos2 = 0;
			pos1 = summands[i].find('*',pos1);
			temp_str = summands[i].substr(0,pos1);
			if (i<2)
				compare_str = argument;
			else
			{	compare_str = argument;
			compare_str.append("^");
			compare_str.append(IntToString(i));
			}
			if ((temp_str!=compare_str)||(pos1==-1))
				return false;

			//check dot expression Pn
			pos1++;
			pos2 = summands[i].size();
			temp_str = summands[i].substr(pos1,pos2-pos1);
			compare_str = "P";
			compare_str.append(IntToString(i));
			if ((temp_str!=compare_str)||(pos2==-1))
				return false;
		}
	}
	return true;

}
bool BezierCurve::CheckDescription(string description)
{
	string name,argument,body;
	unsigned int number_of_points=0,pos1=0,pos2=0;
	number_of_points = NumberOfPointsFromDescription(description);
	if (number_of_points<2)
		return false;
	name = NameFromDescription(description,pos1);
	if (!CheckNameOrArgument(name))
		return false;
	argument = ArgumentFromDescription(description,pos1);
	if (!CheckNameOrArgument(argument))
		return false;
	pos1++;
	if (description[pos1]!='=')
		return false;
	body = BodyFromDescription(description,pos1);

	if (!CheckBody(body,argument,number_of_points))
		return false;

	return true;
}

bool BezierCurve::CheckPointString(string point_string)
{
	int pos1=0,pos2=0;
	pos1 = point_string.find('(',0);
	if(pos1==-1)
		return false;
	pos2 = point_string.find(';',pos1);
	if(pos2==-1)
		return false;
	string x=point_string.substr(pos1+1,pos2-pos1-1);
	if ((!CheckNumberString(x))||(x==""))
		return false;
	pos1 = point_string.find(';',pos2);
	if(pos1==-1)
		return false;
	pos2 = point_string.find(';',pos1+1);
	if(pos2==-1)
		return false;
	string y=point_string.substr(pos1+1,pos2-pos1-1);
	if ((!CheckNumberString(y))||(y==""))
		return false;
	pos1 = point_string.find(';',pos2);
	if(pos1==-1)
		return false;
	pos2 = point_string.find(')',pos1+1);
	if(pos2==-1)
		return false;
	string z=point_string.substr(pos1+1,pos2-pos1-1);
	if ((!CheckNumberString(z))||(z==""))
		return false;
	return true;
}


//Conversion methods
string BezierCurve::NameFromDescription(string Description)
{
	int position = Description.find('(',0);
	return Description.substr(0,position);	
}

string BezierCurve::NameFromDescription(string Description, unsigned int& position)
{
	position = Description.find('(',position);

	return Description.substr(0,position);	
}




string BezierCurve::ArgumentFromDescription(string Description)
{
	int pos1 = Description.find('(',0);
	int pos2 = Description.find(')',pos1);
	return Description.substr(pos1+1,pos2-pos1-1);
}

string BezierCurve::ArgumentFromDescription(string Description, unsigned int& position)
{
	int pos1 = Description.find('(',position);
	int pos2 = Description.find(')',pos1);
	position = pos2;
	return Description.substr(pos1+1,pos2-pos1-1);

}

string BezierCurve::BodyFromDescription(string Description)
{

	int pos1 = Description.find('=',0);
	int pos2 = Description.find(' ',pos1);
	return Description.substr(pos1+1,pos2-pos1-1);
}
string BezierCurve::BodyFromDescription(string Description, unsigned int& position)
{

	int pos1 = Description.find('=',position);
	int pos2 = Description.find(' ',pos1);
	position = pos2;
	return Description.substr(pos1+1,pos2-pos1-1);
}


int BezierCurve::NumberOfPointsFromDescription(string Description)
{
	string body=BodyFromDescription(Description);
	return count(body.begin(),body.end(),'P');
}


point_b BezierCurve::PointFromDescription(string point_string)
{

	point_b Dot;
	int pos1 = point_string.find('(',0);
	int pos2 = point_string.find(';',pos1);
	string x=point_string.substr(pos1+1,pos2-pos1-1);
	pos1 = point_string.find(';',pos2);
	pos2 = point_string.find(';',pos1+1);
	string y=point_string.substr(pos1+1,pos2-pos1-1);
	pos1 = point_string.find(';',pos2);
	pos2 = point_string.find(')',pos1+1);
	string z=point_string.substr(pos1+1,pos2-pos1-1);
	Dot.x=strtod(x.c_str(),NULL);
	Dot.y=strtod(y.c_str(),NULL);
	Dot.z=strtod(z.c_str(),NULL);

	return Dot;
}


//Methods for read Bezier function to object
int BezierCurve::ReadBezierFunctionFromFile(string file_path, string func_name, string arg_name)
{
	string line,temp_name, temp_argument;
	int bezier_error = 0;

	fstream* text_file = new fstream(file_path.c_str(),ios::in);
	text_file->seekg(0,ios::beg);
	text_file->clear();
	
	try 
	{
		if (!text_file->is_open())
			throw 1000;

		while(!text_file->eof())
			{
				*text_file>>line;
				temp_name = NameFromDescription(line);
				temp_argument = ArgumentFromDescription(line);
				
				if ((temp_name==func_name) && (temp_argument==arg_name))
				{
					if (!CheckNameOrArgument(temp_name))
						throw 1001;
					if (!CheckNameOrArgument(temp_argument))
						throw 1002;
					if (!CheckDescription(line))
						throw 1003;
					string Dot_string;
					point_b Dot;
					Name = temp_name;
					Argument = temp_argument;
					CurveBody = BodyFromDescription(line);
					NumberOfPoints = NumberOfPointsFromDescription(CurveBody);
					for (int i=0;i<NumberOfPoints;i++)
					{
						*text_file>>Dot_string;
						if(!CheckPointString(Dot_string))
							throw 1005;
						Dot=PointFromDescription(Dot_string);
						Points.push_back(Dot);
					}
					if (Points.size()!=NumberOfPoints)
						throw 1006;
					Curve_Ready=true;
					break;
				}
				else
				{
					char symbol=NULL;
					while((symbol!='\n')&&(!text_file->eof()))
						text_file->get(symbol);
					if (text_file->eof())
						throw 1010;
				}
		}	
	}
	
	catch (int b_error)
	{
				bezier_error=b_error;
				Name = "";
				Argument = "";
				CurveBody = "";
				NumberOfPoints = 0;
				Points.clear();
				Curve_Ready = false;
	}

	if (text_file->is_open())
		text_file->close();
	delete text_file;

	return bezier_error;		

}

int BezierCurve::SetBezierFunction(string new_description, vector<point_b> new_points)
{
	int bezier_error = 0;
	try
	{
		if (!CheckDescription(new_description))
			throw 1003;

		NumberOfPoints = NumberOfPointsFromDescription(new_description);
		if (NumberOfPoints!=new_points.size())
			throw 1006;
		Name = NameFromDescription(new_description);
		Argument = ArgumentFromDescription(new_description);
		CurveBody = BodyFromDescription(new_description);
		Points = new_points;
		Curve_Ready = true;
	}
	catch (int b_error)
	{
		bezier_error=b_error;
		Name = "";
		Argument = "";
		CurveBody = "";
		NumberOfPoints = 0;
		Points.clear();
		Curve_Ready = false;
	}

	return bezier_error;
}

//External Calculation methods
point_b BezierCurve::ÑalculatePoint(double t)
	{
		point_b Dot;
		Dot.x=0;
		Dot.y=0;
		Dot.z=0;
		if (Is_Ready())
		{
			

			int n = NumberOfPoints-1;
			for (int i=0;i<=n;i++)
			{
				Dot.x = Dot.x + polinome(i,n,t) * Points[i].x;
				Dot.y = Dot.y + polinome(i,n,t) * Points[i].y;
				Dot.z = Dot.z + polinome(i,n,t) * Points[i].z;
			}
			
		}
		
		return Dot;
	
		
	}

double BezierCurve::DistancePoints(const point_b FirstPoint, const point_b SecondPoint)
{
	return sqrt(pow((SecondPoint.x-FirstPoint.x),2)+pow((SecondPoint.y-FirstPoint.y),2)+pow((SecondPoint.z-SecondPoint.z),2));
}

double BezierCurve::DistancePointsCurves(BezierCurve* SecondCurve, double FirstArg, double SecondArg)
{
	if ((this->Is_Ready())&&(SecondCurve->Is_Ready()))
	{
		point_b FirstPoint = this->ÑalculatePoint(FirstArg);
		point_b SecondPoint = SecondCurve->ÑalculatePoint(SecondArg);
		return DistancePoints(FirstPoint, SecondPoint);
	}
	else
		return -1;
}


int  BezierCurve::MinDistanceUniformedSearch(BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg)
{
	if ((this->Is_Ready())&&(SecondCurve->Is_Ready()))
	{
		double Temp_FirstArg=0, Temp_SecondArg = 0;
		double MinDistance = DistancePoints(this->ÑalculatePoint(Temp_FirstArg),SecondCurve->ÑalculatePoint(Temp_SecondArg));
		for (Temp_FirstArg=0;Temp_FirstArg<1.01;Temp_FirstArg=Temp_FirstArg+0.01)
			for (Temp_SecondArg=0;Temp_SecondArg<1.01;Temp_SecondArg=Temp_SecondArg+0.01)
			{

				double TempDistance = DistancePoints(this->ÑalculatePoint(Temp_FirstArg),SecondCurve->ÑalculatePoint(Temp_SecondArg));
				if (TempDistance<MinDistance)
				{	
					MinDistance = TempDistance;
					FirstArg = Temp_FirstArg;
					SecondArg = Temp_SecondArg;
				}
			}
			Distance = MinDistance;
			return 0;
	}
	else
		return 1100;
}
int BezierCurve::MinDistanceGradientDescent(BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg,
												   double coefficient, double tolerance, unsigned short deriv_accuracy)
{

	if ((this->Is_Ready())&&(SecondCurve->Is_Ready()))
	{
		int iteration=0;
		double Temp_Distance = Distance;
		double Last_Distance = Distance;
		double Temp_FirstArg = FirstArg;
		double Temp_SecondArg = SecondArg;
		double alpha_coefficient=1.1, beta_coefficient = 0.5;
		grad_b gradient;
		for(iteration=0;iteration<100;iteration++)
		{
			gradient = GradientOfDistance(SecondCurve,Temp_FirstArg,Temp_SecondArg,deriv_accuracy);
			if(abs(gradient.full_grad)<tolerance)
			{
				FirstArg = Temp_FirstArg;
				SecondArg = Temp_SecondArg;
				Distance = Temp_Distance;
				return 0;

			}
			Temp_FirstArg = Temp_FirstArg - coefficient * gradient.first_grad;
			if (Temp_FirstArg<0)
				Temp_FirstArg=0;
			if (Temp_FirstArg>1)
				Temp_FirstArg=1;
			Temp_SecondArg = Temp_SecondArg - coefficient * gradient.second_grad;
			if (Temp_SecondArg<0)
				Temp_SecondArg=0;
			if (Temp_SecondArg>1)
				Temp_SecondArg=1;
			Temp_Distance = DistancePointsCurves(SecondCurve,Temp_FirstArg,Temp_SecondArg);
			if(Temp_Distance>Last_Distance)
				coefficient*=beta_coefficient;
			else
				coefficient*=alpha_coefficient;
			Last_Distance=Temp_Distance;

		}
		return 1111;

	}
	else
		return 1100;
}

int BezierCurve::MinDistanceFull(BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg, double coefficient,double tolerance)
										
{
	int error_b;
	double Temp_FirstArg=0,Temp_SecondArg=0,Temp_Distance=0;
	error_b = MinDistanceUniformedSearch(SecondCurve,Temp_Distance,Temp_FirstArg,Temp_SecondArg);
	if(error_b!=0)
		return error_b;
	Distance=Temp_Distance;
	FirstArg=Temp_FirstArg;
	SecondArg=Temp_SecondArg;

	error_b = MinDistanceGradientDescent(SecondCurve,Temp_Distance,Temp_FirstArg,Temp_SecondArg,coefficient,tolerance,5);
	if (error_b!=0)
		return error_b;
	Distance=Temp_Distance;
	FirstArg=Temp_FirstArg;
	SecondArg=Temp_SecondArg;
	return 0;

}


//Error Functions
string BezierCurve::DescriptError(int bezier_error)
{

	
	switch(bezier_error)
	{
	case 0:
		return "No error";
	case 1000:
		return "Can't open the file. Please, recheck the file path";
	case 1001:
		return "Incorrect name of the function. Please, recheck the function name";
	case 1002:
		return "Incorrect argument of the function. Please, recheck the argument";
	case 1003:
		return "Incorrect description of the function. Please, recheck description of the function";
	case 1004:
		return "Incorrect body of the function. Please, recheck body of the function";
	case 1005:
		return "Incorrect point string. Please, recheck point string";
	case 1006:
		return "Number of points in body of function does not match of number of points in point string";
	case 1010:
		return "Can't find searching function in the file. Please, recheck file content"; 
	case 1100:
		return "Curve is not ready for treatment. Please, setup the curve object";
	case 1111:
		return "Solution did not converge quickly enough";
	default:
		return "Unknown error";
		
	}
	
}
