

#include "ConsoleWrapper.h"

int ConsoleWrapper::SetFunction() {
    std::cout << "Choose function to optimize:\n";
    for ( auto & [funcName,funcHandler] : model_->functionsLibrary()){
        std::cout << funcHandler.getFunctionNumber() << ". " << funcName << "\n";
    }
    std::string help = "Enter the whole number from  1 to " + std::to_string(model_->functionsLibrary().size()) + ": ";
    int commandNumber = InputIntNumber(help, help);
    for (const auto & [funcName,funcHandler] : model_->functionsLibrary()){
        if (funcHandler.getFunctionNumber() == commandNumber) {
            std::cout << "You chosen: " << funcName;
            model_->setFunctionHandler(funcHandler);
        }
    }
    std::cout << std::endl;
    isFunctionInit = true;
    return 0;
}

void ConsoleWrapper::Run() {

//    STOP = 0,
//    SET_FUNCTION = 1,
//    SET_ALGORITHM = 2,
//    SET_STOPPING_CRITERIA = 3,
//    SET_FUNCTION_DOMAIN = 4,
//    SET_MAGNITUDE = 5,
//    SET_START_POINT = 6,
//    SET_ITERATION_COUNT = 7,
//    SET_STEP_SIZE = 8,
//    REQUEST_COUNT = 9,
    bool running = true;

    std::string command;
    int commandNumber = 0;
    while (running){
        std::stringstream ss;
        ss << "1. Set Function " << (isFunctionInit ? "(OK) [" + model_->functionHandler().getFunctionStrView() + "]\n" : "\n")
           << "2. Set Algorithm " << (isAlgorithmInit ? "(OK) ["  + model_->algorithmStrView() + "]\n" : "\n")
           << "3. Set Stopping criterion " << (isStoppingCriteriaInit ? "(OK) ["  + model_->stoppingCriterionStrView() + "]\n" : "\n")
           << "4. Set Function domain " << (isFunctionDomainInit ? "(OK) \n"  : "\n")
           << "5. Set Magnitude " << (isMagnitudeInit ? "(OK) ["  +  std::to_string(model_->magnitude())+ "]\n" : "\n")
           << "6. Set Start Point " << (isStartPointInit ? "(OK) \n" : "\n")
           << "7. Set Iteration count " << (isIterationCountInit ? "(OK) ["  +  std::to_string(model_->iterCount())+  "]\n" : "\n")
           << "8. Set Step size " <<  (isStepSizeInit ? "(OK) [" + std::to_string(model_->alpha()) + "]\n" : "\n")
           << "9. Run model  \n"
           << "0. Quit \n";
        std::cout << ss.str();
        commandNumber = InputIntNumber({"Enter your choice (option number): "},{"Enter your choice as a whole number: "});
        RequestType requestType;
        if (CheckRequestType(commandNumber)) {
            requestType = static_cast<RequestType>(commandNumber);
        } else {
            std::cout << "Incorrect command \n";
            continue;
        }


        switch (requestType) {
            case RequestType::SET_FUNCTION:
                SetFunction();
                break;
            case RequestType::SET_ALGORITHM:
                SetAlgorithm();
                break;
            case RequestType::SET_STOPPING_CRITERIA:
                SetStoppingCriteria();
                break;
            case RequestType::SET_FUNCTION_DOMAIN:
                SetFunctionDomain();
                break;
            case RequestType::SET_START_POINT:
                SetStartPoint();
                break;
            case RequestType::SET_ITERATION_COUNT:
                SetIterationCount();
                break;
            case RequestType::SET_MAGNITUDE:
                SetMagnitude();
                break;
            case RequestType::SET_STEP_SIZE:
                SetStepSize();
                break;
            case RequestType::RUN_MODEL:
                 RunModel();
                 break;
            case RequestType::STOP:
                running = false;
                break;
            default:
                break;


        }
    }
}
void ConsoleWrapper::RunModel() {
    model_->run();

}

bool ConsoleWrapper::CheckRequestType(int commandNumber) const {
    if (commandNumber < 0 || commandNumber >= static_cast<int>(RequestType::REQUEST_COUNT)) {
        return false;
    }
    return true;
}
bool ConsoleWrapper::CheckStoppingCriterionType(int commandNumber) const {
    if (commandNumber < 0 || commandNumber > static_cast<int>(StoppingCriterion::STOPPING_CRITERION_COUNT)) {
        return false;
    }
    return true;
}
bool ConsoleWrapper::CheckAlgorithmType(int commandNumber) const {
    if (commandNumber < 0 || commandNumber > static_cast<int>(Algorithm::ALGORITHM_COUNT)) {
        return false;
    }
    return true;
}


int ConsoleWrapper::SetStoppingCriteria() {
    std::string hint {};
    std::string errorHint = "Enter a whole number:\n";
    std::stringstream ss;
    ss << "Choose stopping criteria:" <<
                 "\n 1. By Gradient Magnitude " <<
                 "\n 2. By Difference between points" <<
                 "\n 3. By Difference between function values\n Your choice: ";
    std::cout << ss.str();
    int commandNumber = InputIntNumber(hint,errorHint);
    StoppingCriterion stoppingCriterion;
    if (CheckStoppingCriterionType(commandNumber)) {
        stoppingCriterion = static_cast<StoppingCriterion>(commandNumber);
    } else {
        std::cout << "Stopping criterion didn't set\n";
        return 1;
    }
    switch (stoppingCriterion) {
        case StoppingCriterion::BY_GRADIENT_MAGNITUDE:
            model_->setStoppingCriterion(StoppingCriterion::BY_GRADIENT_MAGNITUDE);
            break;
        case StoppingCriterion::BY_DELTA_CHANGE_MAGNITUDE:
            model_->setStoppingCriterion(StoppingCriterion::BY_DELTA_CHANGE_MAGNITUDE);
            break;
        case StoppingCriterion::BY_VALUE_CHANGE_MAGNITUDE:
            model_->setStoppingCriterion(StoppingCriterion::BY_VALUE_CHANGE_MAGNITUDE);
            break;
        default:
            std::cout << "Stopping criteria doesn't set.\n";
            break;
    }
    isStoppingCriteriaInit = true;
    std::cout << "You chosen " << model_->stoppingCriterionStrView() << std::endl;
    return  0;
}

int ConsoleWrapper::SetAlgorithm() {
    std::cout << "Choose algorithm:\n 1. Gradient Descent \n 2. Random Search \n";
    int commandNumber = InputIntNumber({});
    Algorithm algorithm;
    if (CheckAlgorithmType(commandNumber)) {
        algorithm = static_cast<Algorithm>(commandNumber);
    } else {
        std::cout << "Algorithm didn't set\n";
        return 1;
    }

    switch (algorithm) {
        case Algorithm::GRADIENT_DESCENT:
            model_->setAlgorimth(Algorithm::GRADIENT_DESCENT);
            break;
        case Algorithm::RANDOM_SEARCH:
            model_->setAlgorimth(Algorithm::RANDOM_SEARCH);
            break;
        default:
            break;
    }
    isAlgorithmInit = true;
    std::cout << "You chosen: " << model_->algorithmStrView() + "\n";


    return 0;
}

int ConsoleWrapper::SetStartPoint() {
    if (!isFunctionInit) { SetFunction();};
    if (!isFunctionDomainInit) { SetFunctionDomain();};
    std::cout << "Enter start point coordinates separated by spaces: ";
    double coordinate = 0;
    Point startPoint = {};
    std::string line;
    getline(std::cin, line);
    std::istringstream iss(line);
    while (iss >> coordinate && startPoint.size() < model_->functionHandler().getDim())
    {
        startPoint.push_back(coordinate);
    }
    isStartPointInit = true;

    return 0;
}

int ConsoleWrapper::SetMagnitude() {

    double magnitude = InputDoubleNumber("Magnitude: ", "No, enter magnitude as a number: ");
    model_->setMagnitude(magnitude);
    isMagnitudeInit = true;

    return 0;
}

int ConsoleWrapper::SetFunctionDomain() {
    isFunctionDomainInit = true;
    return 0;
}

int ConsoleWrapper::SetNumberOfTrials() {
    int numberOfTrials = InputIntNumber("Enter number of trials:","Enter a whole number: ");
    model_->setNumberOfTrials(numberOfTrials);

    return 0;
}

int ConsoleWrapper::SetIterationCount() {
    int iterCount= InputIntNumber("Enter max number of iterations:","Enter a whole number: ");
    model_->setIterCount(iterCount);
    isIterationCountInit = true;
    return 0;
}

int ConsoleWrapper::SetStepSize() {
    double alpha= InputIntNumber("Enter step size","Enter the number: ");
    model_->setAlpha(alpha);
    isStepSizeInit = true;
    return 0;
}



int ConsoleWrapper::InputIntNumber(const std::string &hint, const std::string &errorHint) const {
    std::string input;
    bool isOk = false;
    int number;
    std::cout << hint;
    std::cin >> input;
    while (!isOk){
        try {
            number = std::stoi(input);
            isOk = true;
        } catch (std::invalid_argument const& ex) {
            std::cout << errorHint;
            std::cin >> input;
        }
    }
    return number;
}
double ConsoleWrapper::InputDoubleNumber(const std::string &hint, const std::string &errorHint) const {
    std::string input;
    bool isOk = false;
    double number;
    std::cout << hint;
    std::cin >> input;
    while (!isOk){
        try {
            number = std::stod(input);
            isOk = true;
        } catch (std::invalid_argument const& ex) {
            std::cout << errorHint;
            std::cin >> input;
        }
    }
    return number;
}



