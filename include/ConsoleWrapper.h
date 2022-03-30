
#ifndef GRADIENTDESCENT_CONSOLEWRAPPER_H
#define GRADIENTDESCENT_CONSOLEWRAPPER_H

#include "model.h"
#include <iomanip>
enum class RequestType {
    RUN_MODEL = 0,
    SET_FUNCTION = 1,
    SET_ALGORITHM = 2,
    SET_STOPPING_CRITERIA = 3,
    SET_FUNCTION_DOMAIN = 4,
    SET_MAGNITUDE = 5,
    SET_START_POINT = 6,
    SET_ITERATION_COUNT = 7,
    SET_STEP_SIZE = 8,
    SET_NUMBER_OF_TRIALS = 9,
    STOP = 10,
    REQUEST_COUNT = 11,
};

class ConsoleWrapper {
public:
    ConsoleWrapper(Model * model);
    void Run() ;
    void RunModel();


private:

    int SetFunction() ;
    int SetStoppingCriteria();
    int SetAlgorithm();
    int SetStartPoint();
    int SetMagnitude();
    int SetFunctionDomain();
    int SetNumberOfTrials();
    int SetIterationCount();
    int SetStepSize();

    bool CheckRequestType(int commandNumber) const;
    bool CheckStoppingCriterionType(int commandNumber) const;
    bool CheckAlgorithmType(int commandNumber) const;
    bool CheckBoundsForPoint(const Point &point) const;

    std::string GenerateHelp() const;
    std::string GetStartPointView() const;
    std::string GetDomainView() const;

    double InputDoubleNumber(const std::string &hint = {}, const std::string &errorHint = {}) const;
    int InputIntNumber(const std::string &hint = {}, const std::string &errorHint = {}) const;

    Model * model_;
};


#endif //GRADIENTDESCENT_CONSOLEWRAPPER_H
