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

    double xMin,xMax,yMin,yMax;
     std::vector<Domain> domain =  functionHandler_.getFunctionDomain();
    xMin = domain.at(0).first;
    xMax = domain.at(0).second;

    yMin = domain.at(1).first;
    yMax=  domain.at(1).second;



    double range_x = xMax - xMin;
    double range_y = yMax - yMin;
    double mx = xMin < 0 ? (range_x / 2) - (range_x / 2) : (range_x / 2);
    double my = yMin < 0 ? (range_y / 2) - (range_y / 2) : (range_y / 2);

    double len = range_x < range_y ? range_x / 2 : range_y / 2;
    double lx,ly,rx,ry,x,y;


    clearState();
    prepareState();
    int iterCount = 0;
    unsigned m_seed  = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator;
    std::uniform_real_distribution<double> uniform_x,uniform_y;
    generator.seed(m_seed);
    PrintIterationLog(iterCount);

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
        PrintIterationLog(iterCount + 1);
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
    double xMin,xMax,yMin,yMax;
     std::vector<Domain> domain =  functionHandler_.getFunctionDomain();
    xMin = domain.at(0).first;
    xMax = domain.at(0).second;

    yMin = domain.at(1).first;
    yMax=  domain.at(1).second;


    double x = currentPoint_.at(0);
    double y = currentPoint_.at(1);
    if (abs(x) >= abs(xMin) || abs(x) >= abs(xMax) || abs(y) >= abs(yMin) || abs(y) >= abs(yMax)) {
        return false;
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



