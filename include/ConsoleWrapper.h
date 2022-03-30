
#ifndef GRADIENTDESCENT_CONSOLEWRAPPER_H
#define GRADIENTDESCENT_CONSOLEWRAPPER_H

#include "model.h"

enum class RequestType {
    STOP = 0,
    CHOOSE_FUNCTION = 1,
    CHOOSE_ALGORITHM = 2,
    CHOOSE_STOPPING_CRITERIA = 3,
    REQUEST_COUNT = 4,
};

class ConsoleWrapper {
public:
    ConsoleWrapper(Model * model) : model_(model) {};
    void Run() ;


private:

    int ChooseFunction() ;
    int ChooseStoppingCriteria();
    int ChooseAlgorithm();
    bool CheckRequestType(int commandNumber) const;
    int InputCommand(const std::string& hint = {}) const;


    Model * model_;
};


#endif //GRADIENTDESCENT_CONSOLEWRAPPER_H
