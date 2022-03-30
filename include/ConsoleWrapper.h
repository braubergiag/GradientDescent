
#ifndef GRADIENTDESCENT_CONSOLEWRAPPER_H
#define GRADIENTDESCENT_CONSOLEWRAPPER_H

#include "model.h"

enum class RequestType {
    STOP = 0,
    SET_FUNCTION = 1,
    SET_ALGORITHM = 2,
    SET_STOPPING_CRITERIA = 3,
    SET_START_POINT = 4,
    SET_MAGNITUDE = 5,
    SET_FUNCTION_DOMAIN = 6,
    SET_ITERATION_COUNT = 7,
    SET_STEP_SIZE = 8,
    REQUEST_COUNT = 9,
};

class ConsoleWrapper {
public:
    ConsoleWrapper(Model * model) : model_(model) {};
    void Run() ;


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


    double InputDoubleNumber(const std::string &hint = {}, const std::string &errorHint = {}) const;
    int InputIntNumber(const std::string &hint = {}, const std::string &errorHint = {}) const;
    bool isFunctionDomainInit {false};
    bool isFunctionInit {false};
    bool isAlgorithmInit{false};
    bool isStartPointInit{false};
    bool isIterationCountInit{false};
    bool isStepSizeInit{false};
    Model * model_;
};


#endif //GRADIENTDESCENT_CONSOLEWRAPPER_H
