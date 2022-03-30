//
// Created by marshall on 30.03.2022.
//

#include "ConsoleWrapper.h"

int ConsoleWrapper::ChooseFunction() {
    std::cout << "Выберите функцию для оптимизации:\n";
    for (const auto & [funcName,funcHandler] : model_->functionsLibrary()){
        std::cout << funcHandler.getFunctionNumber() << ". " << funcName << "\n";

    }
    std::string help = "Введите целое число от 1 до " + std::to_string(model_->functionsLibrary().size()) + ": ";
    int commandNumber = InputCommand(help);
    for (const auto & [funcName,funcHandler] : model_->functionsLibrary()){
        if (funcHandler.getFunctionNumber() == commandNumber) {
            std::cout << "Вы выбрали: " << funcName;
            model_->setFunctionHandler(funcHandler);
        }
    }
    std::cout << std::endl;
    return 0;
}

void ConsoleWrapper::Run() {

    bool running = true;
    std::string command;
    int commandNumber = 0;
    while (running){

        commandNumber = InputCommand();
        RequestType requestType;
        if (CheckRequestType(commandNumber)) {
            requestType = static_cast<RequestType>(commandNumber);
        }


        switch (requestType) {
            case RequestType::CHOOSE_FUNCTION:
                ChooseFunction();
                break;
            case RequestType::CHOOSE_ALGORITHM:
                ChooseAlgorithm();
                break;
            case RequestType::CHOOSE_STOPPING_CRITERIA:
                ChooseStoppingCriteria();
                break;
            case RequestType::STOP:
                running = false;
                break;
            default:
                break;


        }
    }
}

bool ConsoleWrapper::CheckRequestType(int commandNumber) const {
    if (commandNumber < 0 || commandNumber > static_cast<int>(RequestType::REQUEST_COUNT)) {
        return false;
    }
    return true;
}

int ConsoleWrapper::InputCommand(const std::string &hint) const {
    std::string command;
    bool isOk = false;
    int commandNumber;

    std::cout << "Введите номер команды: ";
    std::cin >> command;
    while (!isOk){
        try {
            commandNumber = std::stoi(command);
            isOk = true;
        } catch (std::invalid_argument const& ex) {
            std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
            std::cout << "Введите номер команды: ";
            std::cin >> command;
        }
    }

    return commandNumber;
}

int ConsoleWrapper::ChooseStoppingCriteria() {
    std::cout << "Выберите критерий остановки:"
                 "\n 1. По значению градиента "
                 "\n 2. По значению разности между точками"
                 "\n 3. По значению разности функции";
    std::cout << "Ваш выбор: ";
    std::string choice;
    std::cin >> choice;
    int stoppingCriteriaChoice = std::stoi(choice);
    switch (stoppingCriteriaChoice) {
        case 1:
            model_->setStoppingCriterion(StoppingCriterion::byGradientMagnitude);
            break;
        case 2:
            model_->setStoppingCriterion(StoppingCriterion::byDeltaChangeMagnitude);
            break;
        case 3:
            model_->setStoppingCriterion(StoppingCriterion::byValueChangeMagnitude);
            break;
        default:
            std::cout << "Критерий остановки не выбран.\n";
            break;
    }

    std::cout << "Вы выбрали " << model_->stoppingCriterionStrView() << std::endl;
    return  0;
}

int ConsoleWrapper::ChooseAlgorithm() {
    std::cout << "Выберите алгоритм оптимизации:\n 1. Градиентный спуск \n 2. Случайный поиск \n";
    bool isSuccess = true;
    int commandNumber = InputCommand();
    switch (commandNumber) {
        case 1:
            model_->setAlgorimth(Algorithm::gradientDescent);
            break;
        case 2:
            model_->setAlgorimth(Algorithm::randomSearch);
            break;
        default:
            isSuccess = false;

            break;
    }
    if (isSuccess) {
        std::cout << "Вы выбрали " << model_->algorithmStrView() + "\n";
    } else {
        std::cout << "Алгоритм не выбран.\n";
    }

    return 0;
}
