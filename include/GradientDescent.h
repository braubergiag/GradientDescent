

#ifndef GRADIENTDESCENT_GRADIENTDESCENT_H
#define GRADIENTDESCENT_GRADIENTDESCENT_H
#include <vector>
#include <functional>
#include <cmath>


using Function = std::function<double(const std::vector<double> &)>;
using Point = std::vector<double>;


class GradientDescent {
public:
    GradientDescent();
    ~GradientDescent();
    void InitFunctions(const Function &  objectFunc,const std::vector<Function>& gradFunc);
    bool Optimize(std::vector<double> & funcLoc, double & funcVal);
    void Init(const Point& startPoint, double stepSize, uint maxIterations,double gradientThresh);


private:
    double ComputeGradient (int dim);
    double ComputeGradientAnalytical(int dim);
    std::vector<double> ComputeGradientVector();
    double ComputeGradientMagnitude(std::vector<double> gradientVector);

    std::vector<double> startPoint_;
    std::vector<double> currentPoint_;
    std::vector<Point> history_;

    int nDims_;
    double alpha_;
    int maxIter_;
    double h_;
    double gradientThresh_;

    Function objectFunc_;
    std::vector<Function> gradFunc_;

};

#endif //GRADIENTDESCENT_GRADIENTDESCENT_H
