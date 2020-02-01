/**
 * Akiner - crossplatform Qt-based app which helps you to delete, move or copy akin files.
 * Copyright (C) 2020  Polyakov Daniil
 * Email address: arjentix@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

    void _setupIcon();
    void _setupMenu();
    void _setupLeft();
    void _setupCentral();
    void _setupRight();

    bool _checkLeft();
    bool _checkCentral();
    bool _checkRight();
};
#endif // MAINWINDOW_H
