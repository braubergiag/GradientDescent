#include "gradientdescent.h"




GradientDescent::GradientDescent() {

    nDims_ = 0;
    alpha_ = 0.0;
    maxIter_ = 1;
    h_ = 1e-09;
    eps_ = 1e-09;
}

GradientDescent::~GradientDescent() {

}

void GradientDescent::InitFunctions(const Function &objectFunc, const std::vector<Function> &gradFunc)
{
     objectFunc_ = objectFunc;
     gradFunc_ = gradFunc;

}


bool GradientDescent::Optimize(std::vector<double> & funcLoc, double & funcVal) {

    currentPoint_ = startPoint_;
    int iterCount = 0;
    stoppingMagnitude_ = 1;




    clearState();
    prepareState();

    InitStoppingCriterion();
    PrintIterationLog(iterCount);
    while ((iterCount < maxIter_) && (stoppingMagnitude_ > eps_) && inDomain()) {
        gradientVector_ = ComputeGradientVector();
        stoppingMagnitude_ = (this->*evalMagnitude)();

        std::vector<double> newPoint(nDims_,0);
        for (int i = 0; i < nDims_; ++i) {
            newPoint[i]  += currentPoint_[i] - (alpha_ * gradientVector_[i]);
            historyByCoord_.at(i).push_back(newPoint[i]);

        }
        history_.push_back(currentPoint_);
        prevPoint_ = currentPoint_;
        currentPoint_ = newPoint;
        functionValuesHistory_.push_back(objectFunc_(currentPoint_));

        PrintIterationLog(iterCount + 1);
        iterCount++;


    }
   funcLoc = currentPoint_;
   funcVal = objectFunc_(currentPoint_);
   return 0;
}

bool GradientDescent::randomSearch(std::vector<double> &funcLoc,
                                   double &funcVal,FunctionHandler & fh)
{

     std::vector<Domain> domain =  functionHandler_.getFunctionDomain();

    std::vector<double> ranges = {},middlePoint = {};

    double coordinateRange = 0, minRange = 10e6, singleMiddlePoint = 0;
    for (auto i = 0; i < nDims_; ++i) {
        coordinateRange = domain.at(i).second - domain.at(i).first;
        if (coordinateRange < minRange) {
            minRange = coordinateRange;
        }
        ranges.push_back(coordinateRange);
    }
    for (auto i = 0; i < nDims_; ++i) {
        singleMiddlePoint =  domain.at(i).first < 0 ? (ranges.at(i) / 2) - (ranges.at(i) / 2) : (ranges.at(i) / 2);
        middlePoint.push_back(singleMiddlePoint);
    }
    
    double currentBoardLen = minRange / 2;

    clearState();
    prepareState();
    int iterCount = 0;
    unsigned m_seed  = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator;
    std::uniform_real_distribution<double> uniform;
    generator.seed(m_seed);
    PrintIterationLog(iterCount);
    std::vector<std::pair<double,double>> borders;
    while (iterCount < maxIter_ && currentBoardLen > eps_) {
        for (auto i = 0; i < nDims_; ++i) {
            borders.push_back(std::make_pair(middlePoint.at(i) - currentBoardLen, middlePoint.at(i) + currentBoardLen));

        }
        Point point = {};
        for (auto trialCounter = 0; trialCounter < numberOfTrials_; ++ trialCounter) {
            point.clear();
            for (auto i = 0; i < nDims_; ++i) {
                uniform = std::uniform_real_distribution<double> (borders.at(i).first,borders.at(i).second);
                point.push_back(uniform(generator));
            }
            if (objectFunc_(point) < objectFunc_(middlePoint)) {

                middlePoint = point;
                for (auto i = 0; i < nDims_; ++i) {
                    historyByCoord_.at(i).push_back(middlePoint.at(i));
                }

                history_.push_back(middlePoint);
                currentPoint_ = middlePoint;

                functionValuesHistory_.push_back(objectFunc_(currentPoint_));

            }
        }
        PrintIterationLog(iterCount + 1);
        iterCount++;
        currentBoardLen *= 0.9;
    }
    funcLoc = middlePoint;
    funcVal = objectFunc_(funcLoc);
    return 0;

}

void GradientDescent::Init(const Point &startPoint, double stepSize,
                           uint maxIterations,double magnitude )
{
    startPoint_ = startPoint;
    nDims_ = startPoint.size();
    alpha_ = stepSize;
    maxIter_ = maxIterations;
    eps_ = magnitude;

}


double GradientDescent::ComputeGradientAnalytical(int dim) {
    std::vector<double> newPoint = currentPoint_;
    newPoint[dim] += h_;
    return gradFunc_.at(dim)(newPoint);

}

std::vector<double> GradientDescent::ComputeGradientVector() {
    std::vector<double> gradientVector(nDims_,0);
    for (int i = 0; i < nDims_; ++i){
        gradientVector[i] = ComputeGradientAnalytical(i);
    }
    return  gradientVector;
}

double GradientDescent::ComputeGradientMagnitude() {
    double vectorMagnitude = 0.0;
    for (int i = 0; i < nDims_; ++i) {
        vectorMagnitude += gradientVector_[i] * gradientVector_[i];
    }
    return sqrt(vectorMagnitude);

}

double GradientDescent::ComputeDeltaChangeMagnitude()
{
    double vectorMagnitude = 0.0;
    if (history_.size() < 2) {
        return stoppingMagnitude_;
    }
    for (auto i = 0; i < nDims_; ++i) {
        vectorMagnitude += (currentPoint_[i] - prevPoint_[i]) * (currentPoint_[i] - prevPoint_[i]);
    }
    return sqrt(vectorMagnitude);
}

double GradientDescent::ComputeValueChangeMagnitude()
{
    double vectorMagnitude = 0;
    if (history_.size() < 2) {
        return stoppingMagnitude_;
    }

    vectorMagnitude = (objectFunc_(currentPoint_) - objectFunc_(prevPoint_))
                    * (objectFunc_(currentPoint_) - objectFunc_(prevPoint_)) ;

    return sqrt(vectorMagnitude);
}

void GradientDescent::InitStoppingCriterion()
{


    switch (stoppingCriterion_) {
        case StoppingCriterion::BY_DELTA_CHANGE_MAGNITUDE:
            evalMagnitude = &GradientDescent::ComputeDeltaChangeMagnitude;
            break;
        case StoppingCriterion::BY_VALUE_CHANGE_MAGNITUDE:
            evalMagnitude =&GradientDescent::ComputeValueChangeMagnitude;
            break;
        case StoppingCriterion::BY_GRADIENT_MAGNITUDE:
            evalMagnitude = &GradientDescent::ComputeGradientMagnitude;
            break;
        default:
            perror("Stopping criteria does not set.");

    }
}

void GradientDescent::PrintIterationLog(int iter)
{

    std::cout << "Iter " << iter << " ";
    for (auto v : currentPoint_) {
          std::cout  <<  v << " " ;
    }
    std::cout << " " << objectFunc_(currentPoint_);
    std::cout << std::endl;
}

const FunctionHandler &GradientDescent::functionHandler() const
{
    return functionHandler_;
}

void GradientDescent::setFunctionHandler(const FunctionHandler &newFunctionHandler)
{
    functionHandler_ = newFunctionHandler;
}



uint GradientDescent::numberOfTrials() const
{
    return numberOfTrials_;
}

void GradientDescent::clearState()
{
    history_.clear();
    historyByCoord_.clear();
    functionValuesHistory_.clear();
}

void GradientDescent::prepareState()
{
    history_.push_back(startPoint_);
    currentPoint_ = startPoint_;
    for (auto i = 0; i < nDims_; ++i) {
        historyByCoord_.push_back(std::vector<double>{});
        historyByCoord_[i].push_back(startPoint_[i]);
    }
}

bool GradientDescent::inDomain()
{
    std::vector<Domain> domain =  functionHandler_.getFunctionDomain();
    for (auto i = 0; i < domain.size(); ++i) {
        if ( std::abs(currentPoint_.at(i)) > std::abs(domain.at(i).first) || std::abs(currentPoint_.at(i)) > std::abs(domain.at(i).second)) {
            return false;
        }
    }


    return true;
}

void GradientDescent::setNumberOfTrials(uint newNumberOfTrials)
{
    numberOfTrials_ = newNumberOfTrials;
}

const std::vector<double> &GradientDescent::functionValuesHistory() const
{
    return functionValuesHistory_;
}

const std::vector<std::vector<double> > &GradientDescent::historyByCoord() const
{
    return historyByCoord_;
}

void GradientDescent::setStoppingCriterion(StoppingCriterion newStoppingCriterion)
{
    stoppingCriterion_ = newStoppingCriterion;
}

const std::vector<Point> &GradientDescent::history() const
{
    return history_;
}



