#include "GradientDescent.h"


GradientDescent::GradientDescent() {

    nDims_ = 0;
    alpha_ = 0.0;
    maxIter_ = 1;
    h_ = 0.001;
    gradientThresh_ = 1e-09;
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
    double gradientMagnitude = 1.0;

    while ((iterCount < maxIter_) && (gradientMagnitude > gradientThresh_)) {
        std::vector<double> gradientVector = ComputeGradientVector();
        gradientMagnitude = ComputeGradientMagnitude(gradientVector);

        std::vector<double> newPoint  = currentPoint_;
        for (int i = 0; i < nDims_; ++i) {
            newPoint[i]  += -(gradientVector[i] * alpha_);
        }
        history_.push_back(currentPoint_);
        currentPoint_ = newPoint;
        iterCount++;
    }
    funcLoc = currentPoint_;
    funcVal = objectFunc_(currentPoint_);


    return 0;
}

void GradientDescent::Init(const Point &startPoint, double stepSize,
                           uint maxIterations,double gradientThresh )
{
    startPoint_ = startPoint;
    nDims_ = startPoint.size();
    alpha_ = stepSize;
    maxIter_ = maxIterations;
    gradientThresh_ = gradientThresh;
}


double GradientDescent::ComputeGradientAnalytical(int dim) {
    std::vector<double> newPoint = currentPoint_;
    newPoint[dim] += h_;
    return gradFunc_.at(dim)(newPoint);

}

std::vector<double> GradientDescent::ComputeGradientVector() {
    std::vector<double> gradientVector = currentPoint_;
    for (int i = 0; i < nDims_; ++i){
        gradientVector[i] = ComputeGradientAnalytical(i);
    }

    return  gradientVector;
}

double GradientDescent::ComputeGradientMagnitude(std::vector<double> gradientVector) {
    double vectorMagnitude = 0.0;
    for (int i = 0; i < nDims_; ++i) {
        vectorMagnitude += gradientVector[i] * gradientVector[i];
    }
    return sqrt(vectorMagnitude);

}



