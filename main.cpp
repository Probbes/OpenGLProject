#include "Application.h"

int main()
{
    Application app;
    app.init();
    app.initObj();
    app.loop();
    app.cleanup();
    
    return 0;
}
