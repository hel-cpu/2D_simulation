

#include "Application.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200

int main() {
    Application app(WINDOW_WIDTH, WINDOW_HEIGHT, "Bouncing Balls");
    app.run();
}