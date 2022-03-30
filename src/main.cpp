

#include "model.h"
#include "ConsoleWrapper.h"





int main(int argc, char *argv[])
{

    Model * model = new Model();
    ConsoleWrapper consoleWrapper(model);
    consoleWrapper.Run();
    delete model;
    return 0;

}
