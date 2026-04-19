#include "utils/TextUtils.h"
#include "ui/UISwitcher.h"
#include "core/Game.h"
#include <iostream>
#include <memory>

bool XOGame(int argc, char** argv) {
    std::unique_ptr<IGameUI> ui;
    if (!UISwitcher::initWindow(ui, argc, argv)) {
        return false;
    }
    Game game(ui.get());
    return game.play();
}

int main(int argc, char* argv[]) {
    while (XOGame(argc, argv)) {}

    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
