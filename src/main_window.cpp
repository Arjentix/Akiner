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

#include "main_window.h"

#include <QMessageBox>
#include <vector>

#include "ui_main_window.h"
#include "open_dir_pushbutton.h"
#include "executor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _setupIcon();
    _setupMenu();
    _setupLeft();
    _setupCentral();
    _setupRight();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::helpAction_triggered()
{
    QString text = "Akiner - кроссплатформенное приложение, которое поможет вам удалять, "
                   "перемещать или копировать похожие файлы.\n"
                   "Похожими считаются файлы, имеющие одинаковые названия, "
                   "но без учета полного пути и расширения файла.\n"
                   "\n"
                   "Эта программа является свободным программным обеспечением: "
                   "вы можете распространять и/или модифицировать ее под условиями "
                   "GNU Affero General Public License, опубликованной Free Software Foundation, "
                   "любой 3 версии Лицензии или (на ваш выбор) любой более поздней версии.\n"
                   "\n"
                   "Эта программа распространяется в надежде на то, что она будет полезна, "
                   "но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемой ТОВАРНОЙ ПРИГОДНОСТИ или "
                   "ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННОЙ ЦЕЛИ. Смотрите GNU Affero General Public License "
                   "для получения более подробной информации.\n"
                   "\n"
                   "Copyright (C) 2020 Поляков Даниил\n"
                   "Email: arjentix@gmail.com\n"
                   "Исходный код может быть найден по ссылке: https://github.com/Arjentix/Akiner";
    QMessageBox::information(this, tr("Справка"), text);
}

void MainWindow::on_comboBox_activated(int n)
{
    bool enabled = true;
    if (n == 0) {
        enabled = false;
    }

    _ui->rightLineEdit->setEnabled(enabled);
    _rightOpenDirButton->setEnabled(enabled);
}

void MainWindow::on_plusPushButton_clicked()
{
    // Adding pair of QLineEdit and OpenDirPushButton to the central layout

    QLineEdit* lineEdit = new QLineEdit();
    OpenDirPushButton* openDirButton = new OpenDirPushButton(nullptr, lineEdit);
    QHBoxLayout* horizontal = new QHBoxLayout();
    horizontal->addWidget(lineEdit);
    horizontal->addWidget(openDirButton);

    _ui->centralVerticalLayout->insertLayout(
        _ui->centralVerticalLayout->count() - 2,
        horizontal
    );
}

void clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
           delete item->widget();
        }
        delete item;
    }
}

QLineEdit* layoutItemToLineEdit(QLayoutItem* item) {
    return qobject_cast<QLineEdit*>(item->widget());
}

void MainWindow::on_minusPushButton_clicked()
{
    QLayout* central_layout = _ui->centralVerticalLayout;
    if (central_layout->count() > 5) {
        clearLayout(central_layout->takeAt(central_layout->count() - 3)->layout());
    }
}

void MainWindow::on_runPushButton_clicked()
{
    bool check = _checkLeft() && _checkCentral() && _checkRight();
    if (check != true) {
        return;
    }

    QLayout* layout = _ui->centralVerticalLayout;
    std::vector<std::string> searchFolders;
    searchFolders.reserve(layout->count() - 4);
    for (int i = 2; i < layout->count() - 2; ++i) {
        QLineEdit* lineEdit = layoutItemToLineEdit(layout->itemAt(i)->layout()->itemAt(0));
        if (lineEdit != nullptr) {
            searchFolders.push_back(lineEdit->text().toStdString());
        }
    }

    std::string sourceFolder = _ui->leftLineEdit->text().toStdString();
    std::string destFolder = _ui->rightLineEdit->text().toStdString();

    try {
    switch (_ui->comboBox->currentIndex()) {
    case 0:
        Executor::removeFiles(sourceFolder, searchFolders);
        break;
    case 1:
        Executor::moveFiles(sourceFolder, searchFolders, destFolder);
        break;
    case 2:
        Executor::copyFiles(sourceFolder, searchFolders, destFolder);
        break;
    default:
        break;
    }

    QMessageBox::information(this, tr("Готово"), tr("Все файлы были обработаны"));
    }
    catch (std::exception& ex) {
        QMessageBox::critical(this, tr("Ошибка"), tr(ex.what()));
    }
}

void MainWindow::_setupIcon()
{
    QPixmap appPixmap(":/icons/Akiner.ico");
    QIcon appIcon(appPixmap);
    setWindowIcon(appIcon);
}

void MainWindow::_setupMenu()
{
    _helpAction = new QAction(tr("Справка"), this);
    _ui->menubar->addAction(_helpAction);
    connect(_helpAction, SIGNAL(triggered()), this, SLOT(helpAction_triggered()));
}

void MainWindow::_setupLeft()
{
    // Left 'Open Directory' Button
    _leftOpenDirButton = new OpenDirPushButton(this, _ui->leftLineEdit);
    _ui->leftHorizontalLayout->addWidget(_leftOpenDirButton);
}

void MainWindow::_setupCentral()
{
    // Central 'Open Directory' Button
    _centralOpenDirButton = new OpenDirPushButton(this, _ui->centralLineEdit);
    _ui->centralHorizontalLayout->addWidget(_centralOpenDirButton);

    // Minus button
    QPixmap minusPixmap(":/icons/minus.png");
    QIcon minusIcon(minusPixmap);
    _ui->minusPushButton->setIcon(minusIcon);
    _ui->minusPushButton->setIconSize(minusPixmap.rect().size());
    _ui->minusPushButton->setFixedSize(35, 35);

    // Plus button
    QPixmap plusPixmap(":/icons/plus.png");
    QIcon plusIcon(plusPixmap);
    _ui->plusPushButton->setIcon(plusIcon);
    _ui->plusPushButton->setIconSize(plusPixmap.rect().size());
    _ui->plusPushButton->setFixedSize(35, 35);
}

void MainWindow::_setupRight()
{
    // Right LineEdit
    _ui->rightLineEdit->setEnabled(false);

    // Right 'Open Directory' Button
    _rightOpenDirButton = new OpenDirPushButton(this, _ui->rightLineEdit);
    _rightOpenDirButton->setEnabled(false);
    _ui->rightHorizontalLayout->addWidget(_rightOpenDirButton);
}

bool MainWindow::_checkLeft()
{
    if (_ui->leftLineEdit->text() == "") {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не выбрана директория отобранных файлов!"));
        return false;
    }

    return true;
}

bool MainWindow::_checkCentral()
{
    bool centralLineEditsAreOk = true;
    for (int i = 2; i < _ui->centralVerticalLayout->count() - 2; ++i) {
        QLineEdit* lineEdit = layoutItemToLineEdit(_ui->centralVerticalLayout->itemAt(i)->layout()->itemAt(0));
        if (lineEdit == nullptr || lineEdit->text() == "") {
            centralLineEditsAreOk = false;
            break;
        }
    }
    if (!centralLineEditsAreOk) {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не выбрана директория директория для поиска!"));
        return false;
    }

    return true;
}

bool MainWindow::_checkRight()
{
    if (_ui->comboBox->currentIndex() == 1 && _ui->rightLineEdit->text() == "") {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не выбрана директория для перемещения"));
        return false;
    }
    if (_ui->comboBox->currentIndex() == 2 && _ui->rightLineEdit->text() == "") {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не выбрана директория для копирования"));
        return false;
    }

    return true;
}
