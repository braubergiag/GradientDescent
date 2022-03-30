#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H


#include <vector>
#include <functional>
#include <cmath>
#include <iostream>
#include <chrono>
#include <utility>
#include <map>
#include <random>
#include "functionhandler.h"

using Function = std::function<double(const std::vector<double> &)>;
using Point = std::vector<double>;

enum class StoppingCriterion {
    byGradientMagnitude,
    byDeltaChangeMagnitude,
    byValueChangeMagnitude
};


class GradientDescent {
public:
    GradientDescent();
    ~GradientDescent();
    void InitFunctions(const Function &  objectFunc,const std::vector<Function>& gradFunc);
    void Init(const Point& startPoint, double stepSize, uint maxIterations,double magnitude);
    bool Optimize(std::vector<double> & funcLoc, double & funcVal);
    bool randomSearch(std::vector<double> & funcLoc, double & funcVal,FunctionHandler & fh);

    // Setters
    void setStoppingCriterion(StoppingCriterion newStoppingCriterion);
    void setNumberOfTrials(uint newNumberOfTrials);

    // Getters
    const std::vector<Point> &history() const;
    const std::vector<std::vector<double> > &historyByCoord() const;
    const std::vector<double> &functionValuesHistory() const;
    uint numberOfTrials() const;

    void clearState();
    void prepareState();
    bool inDomain();

    const FunctionHandler &fh() const;

    void setFh(const FunctionHandler &newFh);

    const FunctionHandler &functionHandler() const;
    void setFunctionHandler(const FunctionHandler &newFunctionHandler);

private:

    double ComputeGradientAnalytical(int dim);
    std::vector<double> ComputeGradientVector();
    // Stopping Critereis functions
    double ComputeGradientMagnitude();
    double ComputeDeltaChangeMagnitude();
    double ComputeValueChangeMagnitude();
    void InitStoppingCriterion();
    double (GradientDescent:: *evalMagnitude)();


    void PrintIterationLog(int iter);



    int nDims_;
    double alpha_;
    int maxIter_;
    double h_;
    double eps_;
    double stoppingMagnitude_ = 1.0;
    uint numberOfTrials_;




    StoppingCriterion stoppingCriterion_;
    Point startPoint_,currentPoint_, prevPoint_;
    std::vector<Point> history_;
    std::vector<std::vector<double>> historyByCoord_;
    std::vector<double> functionValuesHistory_;
    std::vector<double> gradientVector_;
    Function objectFunc_;
    std::vector<Function> gradFunc_;
    FunctionHandler functionHandler_;

};


#endif // GRADIENTDESCENT_H
