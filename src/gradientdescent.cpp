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

    history_.clear();
    history_.push_back(startPoint_);
    historyByCoord_.clear();
    functionValuesHistory_.clear();
    for (auto i = 0; i < nDims_; ++i) {
        historyByCoord_.push_back(std::vector<double>{});
        historyByCoord_[i].push_back(startPoint_[i]);
    }

    InitStoppingCriterion();
    while ((iterCount < maxIter_) && (stoppingMagnitude_ > eps_)) {
        gradientVector_ = ComputeGradientVector();
        stoppingMagnitude_ = (this->*evalMagnitude)();

        std::vector<double> newPoint(nDims_,0);
        for (int i = 0; i < nDims_; ++i) {
            newPoint[i]  += currentPoint_[i] - (alpha_ * gradientVector_[i]);
            historyByCoord_[i].push_back(newPoint[i]);
        }
        history_.push_back(currentPoint_);
        prevPoint_ = currentPoint_;
        currentPoint_ = newPoint;
        functionValuesHistory_.push_back(objectFunc_(currentPoint_));

        PrintIterationLog(iterCount);
        iterCount++;


    }
   funcLoc = currentPoint_;
   funcVal = objectFunc_(currentPoint_);
   return 0;
}

bool GradientDescent::randomSearch(std::vector<double> &funcLoc,
                                   double &funcVal,FunctionHandler & fh)
{

    double range_x = fh.getFunctionDomain().at(0).second - fh.getFunctionDomain().at(0).first;
    double range_y = fh.getFunctionDomain().at(1).second - fh.getFunctionDomain().at(1).first;
    double mx = range_x / 2;
    double my = range_y / 2;
    double len = mx < my ? mx : my;
    int iterCount = 0;
    double lx,ly,rx,ry,x,y;


    history_.clear();
    history_.push_back(startPoint_);
    historyByCoord_.clear();
    functionValuesHistory_.clear();
    for (auto i = 0; i < nDims_; ++i) {
        historyByCoord_.push_back(std::vector<double>{});
        historyByCoord_[i].push_back(startPoint_[i]);
    }
    unsigned m_seed  = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator;
    std::uniform_real_distribution<double> uniform_x,uniform_y;
    generator.seed(m_seed);

    while (iterCount < maxIter_ && len > eps_) {
        lx = mx - len;
        rx = mx + len;
        ly = my - len;
        ry = my + len;
        for (auto i = 0; i < numberOfTrials_; ++ i) {
            uniform_x = std::uniform_real_distribution<double> (lx,rx);
            uniform_y = std::uniform_real_distribution<double> (ly,ry);
            x = uniform_x(generator);
            y = uniform_y(generator);

            if (objectFunc_({x,y}) < objectFunc_({mx,my})) {
                mx = x;
                my = y;
                historyByCoord_[0].push_back(x);
                historyByCoord_[1].push_back(y);
                history_.push_back({x,y});
                currentPoint_ = {mx,my};

                functionValuesHistory_.push_back(objectFunc_({x,y}));

            }
        }
        PrintIterationLog(iterCount);
        iterCount++;
        len *= 0.9;
    }
    funcLoc = {mx,my};
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
        case StoppingCriterion::byDeltaChangeMagnitude:
            evalMagnitude = &GradientDescent::ComputeDeltaChangeMagnitude;
            break;
        case StoppingCriterion::byValueChangeMagnitude:
            evalMagnitude =&GradientDescent::ComputeValueChangeMagnitude;
            break;
        case StoppingCriterion::byGradientMagnitude:
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

uint GradientDescent::numberOfTrials() const
{
    return numberOfTrials_;
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



