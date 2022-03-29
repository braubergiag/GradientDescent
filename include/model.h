#ifndef MODEL_H
#define MODEL_H


#include "gradientdescent.h"
#include "functionhandler.h"
#include <iostream>
#include <sstream>

enum class Algorithm {
    gradientDescent,
    randomSearch

};

class Model
{
public:
    Model();

    FunctionHandler& functionHandler();
    Point functionLocation() const;
    const FunctionsLibrary &functionsLibrary() const;
    std::vector<Point> history() const;
    const GradientDescent &gd() const;
    double alpha() const;
    std::string stoppingCriterionStrView() const;
    std::string algorithmStrView() const;
    StoppingCriterion stoppingCriterion() const;
    std::string magnitudeENotation() const;
    int iterCount() const;
    bool isInitialized() const;
    double magnitude() const;
    double functionValue() const;

    void setFunctionLocation(Point newFunctionLocation);
    void setHistory(const std::vector<Point> &newHistory);
    void setFunctionHandler(FunctionHandler newFunctionHandler);
    void setAlpha(double newAlpha);
    void setMagnitudeThreshHold(double newMagnitudeThreshHold);
    void setIterCount(int newIterCount);
    void setStartPoint(Point newStartPoint);
    void setStoppingCriterion(StoppingCriterion newStoppingCriterion);
    void setMagnitude(double newMagnitude);
    void setMagnitudeENotation(const std::string &newMagnitudeENotation);
    void setFunctionsLibrary();
    void setIsInitialized(bool newIsInitialized);







   std::string generateLog();
   void printResult();
   void run();

   uint numberOfTrials() const;
   void setNumberOfTrials(uint newNumberOfTrials);

   Algorithm algorimth() const;
   void setAlgorimth(Algorithm newAlgorimth);

private:
    GradientDescent gd_;
    FunctionHandler  functionHandler_;
    std::vector<Point> history_;
    StoppingCriterion stoppingCriterion_;
    Algorithm algorimth_;
    std::string magnitudeENotation_;
    Point startPoint_;
    Point functionLocation_;
    FunctionsLibrary functionsLibrary_;


    uint numberOfTrials_;
    double functionValue_ {0};
    double magnitude_ {0};
    double alpha_{0};
    double magnitudeThreshHold_ {0};
    int iterCount_{0};

    bool isInitialized_;

};

#endif // MODEL_H
