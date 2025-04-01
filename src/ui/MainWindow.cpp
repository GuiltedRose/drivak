#include "ui/MainWindow.h"
#include "ui/UIScreen.h"
#include <QPainter>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Drivak");
    resize(800, 600);
}

void MainWindow::setScreen(std::shared_ptr<UIScreen> screen) {
    currentScreen = std::move(screen);
    update(); // Force redraw
}

void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    if (currentScreen) {
        currentScreen->draw();  // (Optional) draw to QPainter in future
    }
}
