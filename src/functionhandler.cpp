#include "functionhandler.h"


int FunctionHandler::functionCounter_ = 0;

FunctionHandler::FunctionHandler()
{

}

FunctionHandler::FunctionHandler( Function objectFunction,
                                  std::vector<Function> gradFunction,int dim, std::string  functionStrView):
    objectFunction_(objectFunction),gradFunction_(gradFunction),dim_(dim),functionStrView_(functionStrView),
    functionNumber_(++functionCounter_){

    std::cout << "Create fh #" << functionNumber_ << std::endl;
}

int FunctionHandler::getFunctionNumber() const {
    return functionNumber_;
}


std::string FunctionHandler::getFunctionStrView() const
{
    return functionStrView_;
}

void FunctionHandler::setFunctionStrView(const std::string &newFunctionStrView)
{
    functionStrView_ = newFunctionStrView;
}

 Function FunctionHandler::getObjectFunction() const
{
    return objectFunction_;
}

 std::vector<Function> FunctionHandler::getGradFunction() const
{
    return gradFunction_;
}

void FunctionHandler::setObjectFunction(const Function &newObjectFunction)
{
    objectFunction_ = newObjectFunction;
}

void FunctionHandler::setGradFunction(const std::vector<Function> &newGradFunction)
{
    gradFunction_ = newGradFunction;
}

int FunctionHandler::getDim() const
{
    return dim_;
}

void FunctionHandler::setDim(int newDim)
{
    dim_ = newDim;
}

std::vector<Domain> FunctionHandler::getFunctionDomain() const
{
    return functionDomain_;
}

void FunctionHandler::setFunctionDomain(const std::vector<Domain> &newFunctionDomain)
{
    functionDomain_ = newFunctionDomain;
}

Point FunctionHandler::getStartPoint() const
{
    return startPoint_;
}

void FunctionHandler::setStartPoint(Point newStartPoint)
{
    startPoint_ = newStartPoint;
}
