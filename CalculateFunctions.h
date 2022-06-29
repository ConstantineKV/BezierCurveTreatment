#ifndef _CALCULATE_FUNCTIONS
#define _CALCULATE_FUNCTIONS
#include "BezierCurve.h"

class CalculateFunctions
{
    //Outdated class that been used for debugging program. Now program is not anymore need this class
public:
    static double DistancePoints(point_b FirstPoint, point_b SecondPoint);
    static double DistancePointsCurves(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double FirstArg, double SecondArg);

    static grad_b Gradient(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double StartFirstArg, double StartSecondArg, unsigned short accuracy);
    static int MinDistanceUniformedSearch(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg);
    static int MinDistanceGradientDescent(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg,
        double coefficient, double tolerance, unsigned short deriv_accuracy);
    static int MinDistanceFull(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg, double coefficient,
        double tolerance);
};

#endif 