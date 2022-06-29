// BezierCureTreatment.cpp: главный файл проекта.
#include "BezierCurve.h"
#include "CalculateFunctions.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdio>
#include <vector>


//This program allows to create two BezierCurve objects and set them up from text file or from console.
//In case of file, function needs to specified in format: func(arg)=(1-t)^2*P0+2*t*(1-t)*P1+t^2*P2 (3;5;7) (4;8;2) (1;4;7)
//Also it's calculates minimal distance between two curves by combination of uniformed search and gradient descent method

int main()
{

    //initialization of work variables
    int error_b = 0, NumberOfPoints;
    string file_path = "file1.txt", input_file_path = "", input_func_name = "", input_func_arg = "", input_descr = "", point_line = "";
    double FirstArg = 0, SecondArg = 0, Distance = 0, coefficient = 0, tolerance = 0.1;

    //creating curve objects
    BezierCurve* Bez1 = new BezierCurve();
    BezierCurve* Bez2 = new BezierCurve();
    unsigned int choise_menu = 0, choise_setup = 0, choise_path;

    //main cycle
    while (1)
    {

        cout << "   Menu" << endl;                                                                                  //main menu
        cout << "1. Enter First Curve    " << Bez1->GetFunctionNameAndArgument() << endl;
        cout << "2. Enter Second Curve   " << Bez2->GetFunctionNameAndArgument() << endl;
        cout << "3. Calculate Distance between two points" << endl;
        cout << "4. Calculate Minimal Distance" << endl;
        cout << "5. Exit" << endl << endl;
        cin >> choise_menu;
        if (choise_menu == 5)
            break;
        error_b = 0;
        switch (choise_menu)
        {
            bool checking;
            case 1:
            case 2:
                cout << "1. Set from file" << endl;
                cout << "2. Set through console" << endl;
                cin >> choise_setup;

                if (choise_setup == 1)	//setting up from file
                {
                    cout << "Enter function name     [function]([argument])" << endl;
                    cin >> input_func_name;
                    cout << "Enter argument of function  [function]([argument])" << endl;
                    cin >> input_func_arg;
                    cout << "1. Read function from \"file1.txt\"" << endl;
                    cout << "2. Read function from other path" << endl;
                    cin >> choise_path;
                    if (choise_path == 2)
                    {
                        cout << "Enter the file path" << endl;
                        cin >> file_path;
                    }
                    if ((choise_path != 1) && (choise_path != 2))
                        break;
                    if (choise_menu == 1)
                        error_b = Bez1->ReadBezierFunctionFromFile(file_path, input_func_name, input_func_arg);
                    else if (choise_menu == 2)
                        error_b = Bez2->ReadBezierFunctionFromFile(file_path, input_func_name, input_func_arg);

                    if (error_b == 0)
                        cout << "The curve was successfully set up" << endl;
                    else
                        cout << "An error was occurred during setting up the curve. " << Bez1->DescriptError(error_b) << endl;
                }
                if (choise_setup == 2)	//setting up from console
                {
                    vector<point_b> points;
                    cout << "Enter the function description" << endl;
                    cin >> input_descr;
                    checking = Bez1->CheckDescription(input_descr);
                    while (checking == false)
                    {

                        cout << "Wrong description format. Please, enter function description again or enter \"exit\"" << endl;
                        cin >> input_descr;
                        if (input_descr == "exit")
                            break;
                        checking = Bez1->CheckDescription(input_descr);
                    }
                    if (input_descr == "exit")
                        break;
                    NumberOfPoints = Bez1->NumberOfPointsFromDescription(input_descr);
                    for (int i = 0; i < NumberOfPoints; i++)
                    {
                        cout << "Enter " << i << " point in format (x,y,z)" << endl;
                        cin >> point_line;
                        checking = Bez1->CheckPointString(point_line);
                        while (checking == false)
                        {
                            cout << "Wrong point string format. Please, enter point in format (x,y,z) again or enter \"exit\"" << endl;
                            cin >> point_line;
                            if (point_line == "exit")
                                break;
                            checking = Bez1->CheckPointString(point_line);
                        }
                        if (point_line == "exit")
                            break;
                        points.push_back(Bez1->PointFromDescription(point_line));
                    }
                    if (choise_menu == 1)
                        error_b = Bez1->SetBezierFunction(input_descr, points);
                    else if (choise_menu == 2)
                        error_b = Bez2->SetBezierFunction(input_descr, points);
                    if (error_b != 0)
                        cout << Bez1->DescriptError(error_b) << endl;
                }
                break;
            case 3:
                if ((Bez1->Is_Ready() != true) || (Bez2->Is_Ready() != true))                                               //checking curves
                {
                    cout << "Curves are not ready" << endl;
                    break;
                }
                cout << "Enter first argument" << endl;
                cin >> FirstArg;
                cout << "Enter second argument" << endl;
                cin >> SecondArg;
                cout << "Distance = " << Bez1->DistancePointsCurves(Bez2, FirstArg, SecondArg) << endl;
                cout << "FirstArgument = " << FirstArg << endl;
                cout << "SecondArgument = " << SecondArg << endl;
                break;
            case 4:
                if ((Bez1->Is_Ready() != true) || (Bez2->Is_Ready() != true))                                               //checking curves
                {
                    cout << "Curves are not ready" << endl;
                    break;
                }
                cout << "Please, enter lambda-coefficient for Gradient Descent method (0.5 recomends)" << endl;
                cin >> coefficient;
                cout << "Please, enter tolerance for Gradient Descent method (0.1 recomends)" << endl;
                cin >> tolerance;
                error_b = Bez1->MinDistanceFull(Bez2, Distance, FirstArg, SecondArg, coefficient, tolerance);              //calculation of minimal distance
                if (error_b != 0)
                    cout << "An error was occurred during calculating minimal distance. " << Bez1->DescriptError(error_b) << endl;
                else
                {
                    cout << "Minimal distance = " << Distance << endl;
                    cout << "First argument = " << FirstArg << endl;
                    cout << "Second argument = " << SecondArg << endl;
                }
                break;
            default:
                break;

        }
    }

    delete Bez1;                            //destroying curve objects 
    delete Bez2;

}
