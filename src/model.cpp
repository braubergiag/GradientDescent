#include "model.h"

Model::Model()
{
    setFunctionsLibrary();
}
Point Model::functionLocation() const
{
    return functionLocation_;
}
void Model::setFunctionLocation(Point newFunctionLocation)
{
    functionLocation_ = newFunctionLocation;}
std::vector<Point> Model::history() const
{
    return gd_.history();
}

void Model::setHistory(const std::vector<Point> &newHistory)
{
    history_ = newHistory;
}

const GradientDescent &Model::gd() const
{
    return gd_;
}

double Model::alpha() const
{
    return alpha_;
}

std::string Model::stoppingCriterionStrView() const
{
    switch (stoppingCriterion_) {
        case StoppingCriterion::byDeltaChangeMagnitude:
            return "Difference beetween points";
        case StoppingCriterion::byValueChangeMagnitude:
            return "Difference beetween function values";
        case StoppingCriterion::byGradientMagnitude:
            return "Gradient Magnitude";
        default:
            perror("(StrView) Stopping criteria does not set.");
            return "Stopping criteria does not set.";

    }
}

std::string Model::algorithmStrView() const
{
    switch (algorimth_) {
        case Algorithm::gradientDescent:
            return "Gradien Descent";
        case Algorithm::randomSearch:
            return "Random Search";
        default:
            perror("(StrView) Algorithm criteria does not set.");
            return "Algorithm criteria does not set.";

    }
}

void Model::setFunctionHandler( FunctionHandler  newFunctionHandler)
{

    functionHandler_ = newFunctionHandler;
}



void Model::setAlpha(double newAlpha)
{
    alpha_ = newAlpha;
}

void Model::setMagnitudeThreshHold(double newMagnitudeThreshHold)
{
    magnitudeThreshHold_ = newMagnitudeThreshHold;
}

void Model::setIterCount(int newIterCount)
{
    iterCount_ = newIterCount;
}

void Model::run()
{
    gd_.InitFunctions(functionHandler_.getObjectFunction(),functionHandler_.getGradFunction());
    gd_.setFunctionHandler(functionHandler_);
    gd_.Init(startPoint_,alpha_,
             iterCount_,magnitudeThreshHold_);
    gd_.setStoppingCriterion(this->stoppingCriterion_);
    if (algorimth_ == Algorithm::gradientDescent) {
        gd_.Optimize(functionLocation_,functionValue_);
    } else if (algorimth_ == Algorithm::randomSearch) {
        gd_.setNumberOfTrials(numberOfTrials_);
        gd_.randomSearch(functionLocation_,functionValue_,functionHandler_);
    }

    isInitialized_ = true;
    printResult();
}

uint Model::numberOfTrials() const
{
    return numberOfTrials_;
}

void Model::setNumberOfTrials(uint newNumberOfTrials)
{
    numberOfTrials_ = newNumberOfTrials;
}

Algorithm Model::algorimth() const
{
    return algorimth_;
}

void Model::setAlgorimth(Algorithm newAlgorimth)
{
    algorimth_ = newAlgorimth;
}

Point Model::startPoint() const
{
    return startPoint_;
}
void Model::setFunctionsLibrary()
{
    {
        int dim = 2;
        auto f1View = "(1 - x) * (1 - x) + 100 * (y - x * x) * (y - x * x)";
        auto f1 = [](const Point &  functionLoc) {
            double x = functionLoc.at(0);
            double y = functionLoc.at(1);
            return (1 - x) * (1 - x) + 100 * (y - x * x) * (y - x * x);
        };
        auto f1_dx = [](const Point &  functionLoc) {
            double x = functionLoc.at(0);
            double y = functionLoc.at(1);
            return x * (400* x * x + 2) - 400*x*y - 2;
        };
        auto f1_dy = [](const Point &  functionLoc) {
            double x = functionLoc.at(0);
            double y = functionLoc.at(1);
            return 200*y - 200* x * x;
        };

        FunctionHandler fh_1(f1,{f1_dx,f1_dy},dim,f1View);
        std::vector<Domain> domain = {{-2,2},{-2,2}};
        fh_1.setFunctionDomain(domain);
        fh_1.setStartPoint({0,0});
        functionsLibrary_[f1View] = fh_1;
    }

    {


        int dim = 2;
        auto f5View = "x * exp(-|y|^2)";
        auto f5 = [](const Point & functionLoc) {
            double x = functionLoc.at(0);
            double y = functionLoc.at(1);
            return x * exp(-pow(abs(y),2));
        };
        auto f5_dx = [](const Point& functionLoc) {
            double y  = functionLoc.at(1);
            return exp(-pow(abs(y),2));
        };
        auto f5_dy = [](const Point& functionLoc) {
            double x  = functionLoc.at(0);
            double y  = functionLoc.at(1);
            return -2 * x * exp(-pow(y,2)) * y;
        };
        FunctionHandler fh_5(f5,{f5_dx,f5_dy},dim,f5View);
        std::vector<Domain> domain = {{-2,2},{-1,1}};
        fh_5.setFunctionDomain(domain);
        fh_5.setStartPoint({0,0});
        functionsLibrary_[f5View] = fh_5;

    }
    {
        int dim = 2;
        auto f6View = "400 * (pow(sin(x/30),2) + (pow(cos(y/30),2)))";
        auto f6 = [](const Point & functionLoc) {
            double x = functionLoc.at(0);
            double y = functionLoc.at(1);
            return 400 * (pow(sin(x/30),2) + (pow(cos(y/30),2)));
        };
        auto f6_dx = [](const Point& functionLoc) {
            double x  = functionLoc.at(0);
            return 80./3 * sin(x/30.) * cos(x/30.);
        };
        auto f6_dy = [](const Point& functionLoc) {
            double y  = functionLoc.at(1);
            return -80./3 * sin(y/30.) * cos(y/30.);
        };
        FunctionHandler fh_6(f6,{f6_dx,f6_dy},dim,f6View);
        std::vector<Domain> domain = {{-100,100},{-100,100}};
        fh_6.setFunctionDomain(domain);
        fh_6.setStartPoint({0,0});
        functionsLibrary_[f6View] = fh_6;
    }
}

void Model::printResult()

{
        std::cout << "Function: " + functionHandler_.getFunctionStrView() << std::endl;
        std::cout << "Function start point: ";
        for (const auto & loc : startPoint_){
            std::cout << loc << " ";
        }
        std::cout << std::endl;
        std::cout << "Function location: ";
        for (const auto & funcLoc : functionLocation_){
            std::cout << funcLoc << " ";
        }
        std::cout << std::endl;
        std::cout << "Function value: " <<functionValue_ << std::endl;

}
std::string Model::generateLog()
{
    std::string result {};
    result += "|Function: " + functionHandler_.getFunctionStrView() + "\t";
    result += "|Function start point: ";
    for (const auto & loc : startPoint_){
        result += std::to_string(loc) + " ";
    }
    result += "|\t";
    result += "Function location: ";
    for (const auto & funcLoc : functionLocation_){
         result += std::to_string(funcLoc) + " ";
    }
    result += "\t";
    result += "Function value: " + std::to_string(functionValue_) + "\t|";

    result += "\n";
    if (algorimth_ == Algorithm::gradientDescent) {
        result += "|Stopping Criteria: " + stoppingCriterionStrView() + "\t";
        result += "|Step size: " + std::to_string(alpha_) + "|\t";
    } else if (algorimth_ == Algorithm::randomSearch){
        result += "|Method: " + algorithmStrView() + " Num of trials per frame: " + std::to_string(numberOfTrials_);
    }

    result += "|Iterations count: " + std::to_string(gd_.history().size() - 1)+ "|\t";
    return result;
}



bool Model::isInitialized() const
{
    return isInitialized_;
}

double Model::functionValue() const
{
    return functionValue_;
}

int Model::iterCount() const
{
    return iterCount_;
}

StoppingCriterion Model::stoppingCriterion() const
{
    return stoppingCriterion_;
}

double Model::magnitude() const
{
    return magnitude_;
}

void Model::setMagnitude(double newMagnitude)
{
    magnitude_ = newMagnitude;
}

std::string Model::magnitudeENotation() const
{
    return magnitudeENotation_;
}

void Model::setMagnitudeENotation(const std::string &newMagnitudeENotation)
{
    magnitudeENotation_ = newMagnitudeENotation;
}

void Model::setIsInitialized(bool newIsInitialized)
{
    isInitialized_ = newIsInitialized;
}

const FunctionsLibrary &Model::functionsLibrary() const
{
    return functionsLibrary_;
}

void Model::setStartPoint(Point newStartPoint)
{
    startPoint_ = newStartPoint;
}
FunctionHandler& Model::functionHandler()
{
    return functionHandler_;
}

void Model::setStoppingCriterion(StoppingCriterion newStoppingCriterion)
{
    stoppingCriterion_ = newStoppingCriterion;
}


