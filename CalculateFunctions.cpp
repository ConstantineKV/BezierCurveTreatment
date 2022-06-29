#include "CalculateFunctions.h"
#include "BezierCurve.h"
#include <math.h>

double CalculateFunctions::DistancePoints(const point_b FirstPoint, const point_b SecondPoint)
{
    return sqrt(pow((SecondPoint.x - FirstPoint.x), 2) + pow((SecondPoint.y - FirstPoint.y), 2) + pow((SecondPoint.z - SecondPoint.z), 2));
}

double CalculateFunctions::DistancePointsCurves(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double FirstArg, double SecondArg)
{
    if ((FirstCurve->Is_Ready()) && (SecondCurve->Is_Ready()))
    {
        point_b FirstPoint = FirstCurve->ÑalculatePoint(FirstArg);
        point_b SecondPoint = SecondCurve->ÑalculatePoint(SecondArg);
        return DistancePoints(FirstPoint, SecondPoint);
    }
    else
        return -1;
}

grad_b CalculateFunctions::Gradient(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double FirstArg, double SecondArg, unsigned short accuracy)
{
    grad_b gradient_b = { 0, 0, 0 };
    if ((FirstCurve->Is_Ready()) && (SecondCurve->Is_Ready()))
    {
        double step = pow(0.1, accuracy);
        double first_d, second_d;
        //Calculating gradient for first argument
        if ((FirstArg + step <= 1) && (FirstArg - step >= 0))
        {
            first_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg + step, SecondArg);
            second_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg - step, SecondArg);
            gradient_b.first_grad = (first_d - second_d) / (2 * step);
        }
        if (FirstArg + step > 1)
        {
            first_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg, SecondArg);
            second_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg - step, SecondArg);
            gradient_b.first_grad = (first_d - second_d) / step;
        }
        if (FirstArg - step < 0)
        {
            first_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg + step, SecondArg);
            second_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg, SecondArg);
            gradient_b.first_grad = (first_d - second_d) / step;
        }
        //Calculating gradient for second argument
        if ((SecondArg + step <= 1) && (SecondArg - step >= 0))
        {
            first_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg, SecondArg + step);
            second_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg, SecondArg - step);
            gradient_b.second_grad = (first_d - second_d) / (2 * step);
        }
        if (SecondArg + step > 1)
        {
            first_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg, SecondArg);
            second_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg, SecondArg - step);
            gradient_b.second_grad = (first_d - second_d) / step;
        }
        if (SecondArg - step < 0)
        {
            first_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg, SecondArg + step);
            second_d = DistancePointsCurves(FirstCurve, SecondCurve, FirstArg, SecondArg);
            gradient_b.second_grad = (first_d - second_d) / step;
        }
        //Calculating full gradient
        gradient_b.full_grad = sqrt(pow(gradient_b.first_grad, 2) + pow(gradient_b.second_grad, 2));
        return gradient_b;
    }
    else
        return gradient_b;
}
int  CalculateFunctions::MinDistanceUniformedSearch(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg)
{
    if ((FirstCurve->Is_Ready()) && (SecondCurve->Is_Ready()))
    {
        double Temp_FirstArg = 0, Temp_SecondArg = 0;
        double MinDistance = DistancePoints(FirstCurve->ÑalculatePoint(Temp_FirstArg), SecondCurve->ÑalculatePoint(Temp_SecondArg));
        for (Temp_FirstArg = 0; Temp_FirstArg < 1.01; Temp_FirstArg = Temp_FirstArg + 0.01)
            for (Temp_SecondArg = 0; Temp_SecondArg < 1.01; Temp_SecondArg = Temp_SecondArg + 0.01)
            {

                double TempDistance = DistancePoints(FirstCurve->ÑalculatePoint(Temp_FirstArg), SecondCurve->ÑalculatePoint(Temp_SecondArg));
                if (TempDistance < MinDistance)
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
int CalculateFunctions::MinDistanceGradientDescent(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg,
    double coefficient, double tolerance, unsigned short deriv_accuracy)
{

    if ((FirstCurve->Is_Ready()) && (SecondCurve->Is_Ready()))
    {
        int iteration = 0;
        double Temp_Distance = Distance;
        double Last_Distance = Distance;
        double Temp_FirstArg = FirstArg;
        double Temp_SecondArg = SecondArg;
        double alpha_coefficient = 1.1, beta_coefficient = 0.5;
        grad_b gradient;
        for (iteration = 0; iteration < 100; iteration++)
        {
            gradient = Gradient(FirstCurve, SecondCurve, Temp_FirstArg, Temp_SecondArg, deriv_accuracy);
            if (abs(gradient.full_grad) < tolerance)
            {
                FirstArg = Temp_FirstArg;
                SecondArg = Temp_SecondArg;
                Distance = Temp_Distance;
                return 0;

            }
            Temp_FirstArg = Temp_FirstArg - coefficient * gradient.first_grad;
            if (Temp_FirstArg < 0)
                Temp_FirstArg = 0;
            if (Temp_FirstArg > 1)
                Temp_FirstArg = 1;
            Temp_SecondArg = Temp_SecondArg - coefficient * gradient.second_grad;
            if (Temp_SecondArg < 0)
                Temp_SecondArg = 0;
            if (Temp_SecondArg > 1)
                Temp_SecondArg = 1;
            Temp_Distance = DistancePointsCurves(FirstCurve, SecondCurve, Temp_FirstArg, Temp_SecondArg);
            if (Temp_Distance > Last_Distance)
                coefficient *= beta_coefficient;
            else
                coefficient *= alpha_coefficient;
            Last_Distance = Temp_Distance;

        }
        return 1111;

    }
    else
        return 1100;
}

int CalculateFunctions::MinDistanceFull(BezierCurve* FirstCurve, BezierCurve* SecondCurve, double& Distance, double& FirstArg, double& SecondArg, double coefficient,
    double tolerance)
{
    int error_b;
    double Temp_FirstArg = 0, Temp_SecondArg = 0, Temp_Distance = 0;
    error_b = MinDistanceUniformedSearch(FirstCurve, SecondCurve, Temp_Distance, Temp_FirstArg, Temp_SecondArg);
    if (error_b != 0)
        return error_b;
    Distance = Temp_Distance;
    FirstArg = Temp_FirstArg;
    SecondArg = Temp_SecondArg;

    error_b = MinDistanceGradientDescent(FirstCurve, SecondCurve, Temp_Distance, Temp_FirstArg, Temp_SecondArg, coefficient, tolerance, 5);
    if (error_b != 0)
        return error_b;
    Distance = Temp_Distance;
    FirstArg = Temp_FirstArg;
    SecondArg = Temp_SecondArg;
    return 0;

}