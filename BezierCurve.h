#ifndef _BEZIER_CURVE
#define _BEZIER_CURVE
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>


using namespace std;
struct point_b
{
    double x;
    double y;
    double z;
};

struct grad_b
{
    double first_grad;
    double second_grad;
    double full_grad;
};

class BezierCurve
{
private:
    vector <point_b> Points;
    bool Curve_Ready;
    string Name;
    string Argument;
    string CurveBody;
    size_t NumberOfPoints;


public:
    ////////////////////////////CONSTRUCTORS///////////////////////////////////////////////

    //Default constructor. Initializes all variables of object
    BezierCurve();

    /*
    Constructor that makes connection to file and search description of Bezier.

    file_path - system path to the text file
    func_name - name of the function, which program looking for
    arg_name  - argument of function, which program looking for
    error_b   - class error code (can be descripted by DescriptError() method)
    */
    BezierCurve(string file_path, string func_name, string arg_name, int& error_b);

    /*
    Constructor that set up the curve through parameters of description and

    new_description - description of function
    new_points		- vector object of points
    error_b			- class error code (can be descripted by DescriptError() method)
    */
    BezierCurve(string new_description, vector<point_b> new_points, int& error_b);

    /////////////////////////INTERNAL CALCULATION METHODS/////////////////////////////////////

    //returns factorial of number "n"
    unsigned int factorial(unsigned int n);

    //returns binome of i and n numbers
    unsigned int binome(int i, int n);

    /*
    returns Bernstein's polinome
    i - iteration
    n - number of iterations
    t - argument
    */
    double polinome(int i, unsigned long n, double t);

    /*
    returns grad_b structure which constains gradient for first and second argument of distance function and also full gradient

    SecondCurve		- pointer to other curve which is participating in distance function
    StartFirstArg	- first argument of first function (for this object)
    StartSecondArg	- argument of second function
    accuracy		- exponent for step of derivative function. Certain accuracy is 0.1^accuracy
    */
    grad_b GradientOfDistance(BezierCurve* SecondCurve, double StartFirstArg, double StartSecondArg, unsigned short accuracy);

    /////////////////////////INTERNAL SERVICE METHODS/////////////////////////////////////////

    //returns string which is converted from int number
    string IntToString(int number_to_convert);

    //returns string which is converted from double number
    string DoubleToString(double number_to_convert);

    //returns int number which is converted from string
    int StringToInt(string str_to_convert);

    //returns double number which is converted from string
    double StringToDouble(string str_to_convert);


public:

    /////////////////////////STATUS METHODS///////////////////////////////////////////////////

    //Returns curve status. Returns false if curve is not ready for calculating methods
    bool Is_Ready();

    //Returns name of the function
    string GetFunctionName();

    //returns argument of the function
    string GetFunctionArgument();

    //returns name and argument of the function in format "function(argument)"
    string GetFunctionNameAndArgument();

    //returns body of the function
    string GetFunctionBody();

    ////////////////////////CHECK METHODS///////////////////////////////////////////////////////

    //returns true if name_or_argument string is not contains any character but letters
    bool CheckNameOrArgument(string name_or_argument);

    //returns true if number_string is not contains any character but digits
    bool CheckNumberString(string number_string);

    //returns true if body is correct Bezier function body for argument and number_of_points
    bool CheckBody(string body, string argument, size_t number_of_points);

    //returns true if description is correct Bezier function description
    bool CheckDescription(string description);

    //returns true if point_string is point string in format (x;y;z). Example: (10;22.5;13)
    bool CheckPointString(string point_string);


    ////////////////////////CONVERSION METHODS///////////////////////////////////////////////////
    /*
        This methods are intended for conversion of description and point string to
        values which is necessary for setting up BezierCurve object
        Example:
        f(t)=(1-t)*P0+t*P1 (1;2;3)
        f			  - name
        t			  - argument
        (1-t)*P0+t*P1 - body
        (1;2;3)		  - point string
    */


    //returns name of function from Bezier function description. 
    string NameFromDescription(string Description);

    //returns name of function from Bezier function description; position - last symbol position which is searching function stopped
    string NameFromDescription(string Description, size_t& position);

    //returns argument of function from Bezier function description
    string ArgumentFromDescription(string Description);

    //returns argument of function from Bezier function description; position - last symbol position which is searching function stopped
    string ArgumentFromDescription(string Description, size_t& position);

    //returns body of function from Bezier function description
    string BodyFromDescription(string Description);

    //returns body of function from Bezier function description; position - last symbol position which is searching function stopped
    string BodyFromDescription(string Description, size_t& position);

    //returns point_b structure from point string in format (x;y;z). Example: (12;3.4;2)
    point_b PointFromDescription(string point_string);

    //returns number of points from Description if Description is Bezier curve description
    size_t NumberOfPointsFromDescription(string Description);

    ///////////////////////////////////FUNCTION SETUP METHODS///////////////////////////////////////

    /*
        returns class error code; 0 - no error. Method read Bezier function from txt file which is
        determined by file_path.

        file_path - path to the text file which is contains Bezier function and points.
        func_name - name of searching function
        arg_name  - argument of searching function
    */
    int ReadBezierFunctionFromFile(string file_path, string func_name, string arg_name);

    /*
        returns class error code; 0 - no error; Method is sets up Bezier function by parameters

        new_description - description of Bezier function. It has to be in format "fu(arg)=(1-arg)*P0+arg*P1"
        new_points	    - vector point_b structures which is contains points of Bezier function
    */
    int SetBezierFunction(string new_description, vector<point_b> new_points);


    /////////////////////////////////EXTERNAL CALCULATE METHODS////////////////////////////////////

    //returns point calculated from Bezier function with t argument
    point_b ÑalculatePoint(double t);

    //returns distance between two points given by FirstPoint and SecondPoint
    double DistancePoints(point_b FirstPoint, point_b SecondPoint);

    //returns distance between two points given by Bezier curves and arguments FirstArg and SecondArg
    double DistancePointsCurves(BezierCurve* SecondCurve, double FirstArg, double SecondArg);

    /*
      returns class error code; 0 - no error. Method realize uniformed search method for searching minimal distance
      between two Bezier curves.

      SecondCurve - pointer to other Bezier curve
      Distance    - link to distance variable. Returns minimal distance
      FirstArg	  - link to argument of first Bezier function
      SecondArg	  - link to argument of second Bezier function
    */
    int MinDistanceUniformedSearch(BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg);

    /*
      returns class error code; 0 - no error. Method gradient descent search method for searching minimal distance
      between two Bezier curves.

      SecondCurve    - pointer to other Bezier curve
      Distance       - link to distance variable. Returns minimal distance
      FirstArg	     - link to argument of first Bezier function
      SecondArg	     - link to argument of second Bezier function
      coefficient    - lambda-coefficient for gradient descent method
      tolerance		 - permissible value of full gradient for gradient descent method
      deriv_accuracy - exponent for step of derivative function. Certain accuracy is 0.1^accuracy
    */
    int MinDistanceGradientDescent(BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg,
        double coefficient, double tolerance, unsigned short deriv_accuracy);

    /*returns class error code; 0 - no error. Method combines both uniform search and gradient descent methods for searching minimal distance
    between two Bezier curves.
    SecondCurve - pointer to other Bezier curve
    Distance    - link to distance variable. Returns minimal distance
    FirstArg	  - link to argument of first Bezier function
    SecondArg	  - link to argument of second Bezier function
    coefficient - lambda-coefficient for gradient descent method
    tolerance   - permissible value of full gradient for gradient descent method
    */
    int MinDistanceFull(BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg, double coefficient, double tolerance);

    ///////////////////////////////ERROR METHODS////////////////////////////////////////////////////

    //returns error description preassigned by error_b
    string DescriptError(int error_b);

};


#endif