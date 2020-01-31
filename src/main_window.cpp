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

    _setup_left();
    _setup_central();
    _setup_right();
}

MainWindow::~MainWindow()
{
    delete _ui;
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
    if (_check_left() && _check_central() && _check_right() != true) {
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

void MainWindow::_setup_left()
{
    // Left 'Open Directory' Button
    _leftOpenDirButton = new OpenDirPushButton(this, _ui->leftLineEdit);
    _ui->leftHorizontalLayout->addWidget(_leftOpenDirButton);
}

void MainWindow::_setup_central()
{
    // Central 'Open Directory' Button
    _centralOpenDirButton = new OpenDirPushButton(this, _ui->centralLineEdit);
    _ui->centralHorizontalLayout->addWidget(_centralOpenDirButton);

    // Minus button
    QPixmap minus_pixmap(":/icons/minus.png");
    QIcon minusIcon(minus_pixmap);
    _ui->minusPushButton->setIcon(minusIcon);
    _ui->minusPushButton->setIconSize(minus_pixmap.rect().size());
    _ui->minusPushButton->setFixedSize(35, 35);

    // Plus button
    QPixmap plus_pixmap(":/icons/plus.png");
    QIcon plusIcon(plus_pixmap);
    _ui->plusPushButton->setIcon(plusIcon);
    _ui->plusPushButton->setIconSize(plus_pixmap.rect().size());
    _ui->plusPushButton->setFixedSize(35, 35);
}

void MainWindow::_setup_right()
{
    // Right LineEdit
    _ui->rightLineEdit->setEnabled(false);

    // Right 'Open Directory' Button
    _rightOpenDirButton = new OpenDirPushButton(this, _ui->rightLineEdit);
    _rightOpenDirButton->setEnabled(false);
    _ui->rightHorizontalLayout->addWidget(_rightOpenDirButton);
}

bool MainWindow::_check_left()
{
    if (_ui->leftLineEdit->text() == "") {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не выбрана директория отобранных файлов!"));
        return false;
    }

    return true;
}

bool MainWindow::_check_central()
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

bool MainWindow::_check_right()
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
