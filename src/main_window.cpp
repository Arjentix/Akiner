#include "main_window.h"

#include <iostream>

#include "ui_main_window.h"
#include "open_dir_pushbutton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _setup_left();
    _setup_central();
    _setup_right();

    std::cout << "Begin: " << _ui->centralVerticalLayout->count() << std::endl;
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

void MainWindow::on_minusPushButton_clicked()
{
    QLayout* central_layout = _ui->centralVerticalLayout;
    if (central_layout->count() > 5) {
        clearLayout(central_layout->takeAt(central_layout->count() - 3)->layout());
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
    QPixmap minus_pixmap(":/Icons/minus.png");
    QIcon minusIcon(minus_pixmap);
    _ui->minusPushButton->setIcon(minusIcon);
    _ui->minusPushButton->setIconSize(minus_pixmap.rect().size());
    _ui->minusPushButton->setFixedSize(35, 35);

    // Plus button
    QPixmap plus_pixmap(":/Icons/plus.png");
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
