#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_zmqButton_clicked();
    void on_httpButton_clicked();
    void updateResultText(const QString &text);

private:
    Ui::MainWindow *ui;
};

