#pragma once
#include <QMainWindow>
#include <memory>

class UIScreen;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    void setScreen(std::shared_ptr<UIScreen> screen);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::shared_ptr<UIScreen> currentScreen;
};
