
#include "gradientdescent.h"
#include <iostream>
#include <functional>
#include <sstream>
#include <cmath>


void PrintResult(const  Point &functionLocation, double functionValue,
                 Point startPoint,const std::string & funcName = {}){
    std::cout << "Function: " + funcName << std::endl;
    std::cout << "Function start point: ";
    for (const auto & loc : startPoint){
        std::cout << loc << " ";
    }
    std::cout << std::endl;
    std::cout << "Function location: ";
    for (const auto & funcLoc : functionLocation){
        std::cout << funcLoc << " ";
    }
    std::cout << std::endl;
    std::cout << "Function value: " << functionValue << std::endl;

}

void TestF1(){
    GradientDescent gd;
    Point functionLocation;
    Point startPoint;
    double functionValue;
    auto rosenbrockFunc = [](const Point &  functionLoc) {
        double x = functionLoc.at(0);
        double y = functionLoc.at(1);
        return (1 - x) * (1 - x) + 100 * (y - x * x) * (y - x * x);
    };
    auto rosenbrockFunc_dx = [](const Point &  functionLoc) {
        double x = functionLoc.at(0);
        double y = functionLoc.at(1);
        return x * (400* x * x + 2) - 400*x*y - 2;
    };
    auto rosenbrockFunc_dy = [](const Point &  functionLoc) {
        double x = functionLoc.at(0);
        double y = functionLoc.at(1);
        return 200*y - 200* x * x;
    };
    {


        Function func {rosenbrockFunc};
        std::vector<Function> gradientFunc {rosenbrockFunc_dx, rosenbrockFunc_dy};
        startPoint = {2.0,2.0};
        gd.InitFunctions(func,gradientFunc);
        gd.Init(startPoint,0.001,10000, 1e-09);
        gd.setStoppingCriterion(StoppingCriterion::byGradientMagnitude);
        gd.Optimize(functionLocation,functionValue);
        PrintResult(functionLocation,functionValue,startPoint,"F1 Rosenbrock");

    }
}
void TestF2(){
    GradientDescent gd;
    Point functionLocation;
    Point startPoint;
    double functionValue;
    auto f2 = [](const Point &  functionLoc) {
        double x = functionLoc.at(0);
        return (x * x * cos(x) - x) / 10;
    };
    auto f2_dx = [](const Point &  functionLoc) {
        double x = functionLoc.at(0);
        return -1./10*x*x*sin(x) + 1./5*x*cos(x) - 1./10;
    };
    Function func {f2};
    std::vector<Function> gradientFunc {f2_dx};
    gd.InitFunctions(func,gradientFunc);
    {

        startPoint =  {6.0};

        gd.Init(startPoint,0.001,1000, 1e-09);
        gd.setStoppingCriterion(StoppingCriterion::byGradientMagnitude);
        gd.Optimize(functionLocation,functionValue);

        PrintResult(functionLocation,functionValue,startPoint,"F2");
    }
    {
       startPoint =  {8.0};
       gd.Init(startPoint,0.001,1000, 1e-09);
        gd.setStoppingCriterion(StoppingCriterion::byGradientMagnitude);
       gd.Optimize(functionLocation,functionValue);

       PrintResult(functionLocation,functionValue,startPoint,"F2");


    }
}
void TestF3(){
    GradientDescent gd;
    Point functionLocation;
    Point startPoint;
    double functionValue;
    auto f3 = [](const Point &  functionLoc) {
        double x = functionLoc.at(0);
        return pow(x,4) + 2 * pow(x,3) - 6 * x *x + 4*x + 2;
    };
    auto f3_dx = [](const Point &  functionLoc) {
        double x = functionLoc.at(0);
        return 4 * pow(x,3) + 6 * x * x - 12 * x + 4;
    };
    Function func {f3};
    std::vector<Function> gradientFunc {f3_dx};
    gd.InitFunctions(func,gradientFunc);
    {

        startPoint = {20.0};
        gd.Init(startPoint,0.001,1000, 1e-09);
        gd.Optimize(functionLocation,functionValue);
        gd.setStoppingCriterion(StoppingCriterion::byGradientMagnitude);
        PrintResult(functionLocation,functionValue,startPoint,"F3");

    }
}



int main(int argc, char *argv[])
{

      TestF1();
      TestF2();
      TestF3();
      return 0;

}
