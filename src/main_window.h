#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include <utility>

#include "open_dir_pushbutton.h"

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
    void helpAction_triggered();
    void on_comboBox_activated(int n);
    void on_plusPushButton_clicked();
    void on_minusPushButton_clicked();
    void on_runPushButton_clicked();

private:
    Ui::MainWindow* _ui;
    OpenDirPushButton* _leftOpenDirButton;
    OpenDirPushButton* _centralOpenDirButton;
    OpenDirPushButton* _rightOpenDirButton;
    QAction* _helpAction;

    void _setup_menu();
    void _setup_left();
    void _setup_central();
    void _setup_right();

    bool _check_left();
    bool _check_central();
    bool _check_right();
};
#endif // MAINWINDOW_H
