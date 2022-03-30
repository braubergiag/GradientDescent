
#include "gradientdescent.h"
#include "model.h"
#include "functionhandler.h"
#include <iostream>
#include <functional>
#include <sstream>
#include <cmath>

enum class Command {
    CHOOSE_FUNCTION = 1,
    CHOOSE_ALGORITHM = 2,
    CHOOSE_STOPPING_CRITERIA = 3,
};

int FunctionHandler::functionCounter_ = 0;
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

        PrintResult(functionLocation,functionValue,startPoint,"F2 ");
    }
    {
       startPoint =  {8.0};
       gd.Init(startPoint,0.001,1000, 1e-09);
        gd.setStoppingCriterion(StoppingCriterion::byGradientMagnitude);
       gd.Optimize(functionLocation,functionValue);

       PrintResult(functionLocation,functionValue,startPoint,"F2 ");


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


int ChooseStoppingCriteria(Model * model){
    std::cout << "Выберите критерий остановки:"
                 "\n 1. По значению градиента "
                 "\n 2. По значению разности между точками"
                 "\n 3. По значению разности функции";
    std::cout << "Ваш выбор: ";
    std::string choice;
    std::cin >> choice;
    int stoppingCriteriaChoice = std::stoi(choice);
    switch (stoppingCriteriaChoice) {
        case 1:
            model->setStoppingCriterion(StoppingCriterion::byGradientMagnitude);
            break;
        case 2:
            model->setStoppingCriterion(StoppingCriterion::byDeltaChangeMagnitude);
            break;
        case 3:
            model->setStoppingCriterion(StoppingCriterion::byValueChangeMagnitude);
            break;
        default:
            std::cout << "Критерий остановки не выбран.\n";
            break;
    }
    return  0;
}

int ChooseAlgorithm(Model * model){
    std::cout << "Выберите алгоритм оптимизации:\n 1. Градиентный спуск \n 2. Случайный поиск \n";
    std::cout << "Ваш выбор: ";
    std::string algorithmNumberChoice;
    std::cin >> algorithmNumberChoice;
    int algorithmNumberChoiceInt  = std::stoi(algorithmNumberChoice);
    switch (algorithmNumberChoiceInt) {
        case 1:
            model->setAlgorimth(Algorithm::gradientDescent);
            break;
        case 2:
            model->setAlgorimth(Algorithm::randomSearch);
            break;
        default:
            std::cout << "Алгоритм не выбран.\n";
            break;
    }
    return  0;
}

int ChooseFunction(Model * model) {

    std::string command_line;
    std::cout << "Выберите функцию для оптимизации:\n";
    int counter = 0;
    for (auto & f : model->functionsLibrary()){
        std::cout << (counter + 1) << ". " << f.first << "\n";
        ++counter;
    }
    std::cout << "Введите целое число от 1 до " << model->functionsLibrary().size() << ": ";
    std::string funcNumberChoice;
    std::cin >> funcNumberChoice;
    int funcNumber  = std::stoi(funcNumberChoice);
    FunctionHandler fh;
    for (auto & [key,value] : model->functionsLibrary()){
        if (value.getFunctionNumber() == funcNumber) {
            std::cout << "Вы выбрали: " << key;
            fh = model->functionsLibrary().at(key);

        }
    }
    std::cout << std::endl;
    std::cin.clear();
}
int main(int argc, char *argv[])
{

    Model * model = new Model();


    std::string command_line;
    while (getline(std::cin,command_line)) {
        std::stringstream ss(command_line);
        std::string command;
        ss >> command;
        int commandNumber = std::stoi(command);
        switch (commandNumber) {
            case 1:
                ChooseFunction(model);
                break;
            case 2:
                ChooseAlgorithm(model);
                break;
            case 3:
                ChooseStoppingCriteria(model);
                break;
            default:
                break;


        }
    }

    delete model;
      return 0;

}
