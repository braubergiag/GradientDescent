#ifndef FUNCTIONHANDLER_H
#define FUNCTIONHANDLER_H


#include <vector>
#include <functional>
#include <map>
using Function = std::function<double(const std::vector<double> &)>;
using Domain = std::pair<double,double>;
using Point = std::vector<double>;

class FunctionHandler
{
public:
    FunctionHandler();
    FunctionHandler(const Function objectFunction,
                    const std::vector<Function>  gradFunction,int dim=1,const std::string functionStrView = {});


    Function getObjectFunction() const;
    std::vector<Function> getGradFunction() const;
    std::string getFunctionStrView() const;
    int getDim() const;
    std::vector<Domain> getFunctionDomain() const;
    Point getStartPoint() const;

    void setObjectFunction(const Function &newObjectFunction);
    void setGradFunction(const std::vector<Function> &newGradFunction);
    void setFunctionStrView(const std::string &newFunctionStrView);
    void setDim(int newDim);
    void setFunctionDomain(const std::vector<Domain> &newFunctionDomain);
    void setStartPoint(Point newStartPoint);



private:

    std::vector<Domain> functionDomain_; //
    Point startPoint_;
    Function objectFunction_;
    std::vector<Function> gradFunction_;
    int dim_ = 0;
    std::string functionStrView_;




};

using FunctionsLibrary = std::map<std::string, FunctionHandler>;



#endif // FUNCTIONHANDLER_H
