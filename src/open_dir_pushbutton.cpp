#include "open_dir_pushbutton.h"

#include <QFileDialog>
#include <QDir>

OpenDirPushButton::OpenDirPushButton(QWidget* parent, QLineEdit* lineEdit)
    : QPushButton(parent)
    , _lineEdit(lineEdit)
{
    QPixmap pixmap(":/Icons/open_dir.jpeg");
    QIcon buttonIcon(pixmap);
    this->setIcon(buttonIcon);
    this->setIconSize(pixmap.rect().size());
    this->setFixedSize(35, 35);

    connect(this, SIGNAL(clicked()), this, SLOT(open_dir_dialog()));
}

OpenDirPushButton::~OpenDirPushButton() {}

void OpenDirPushButton::open_dir_dialog()
{
    QString dirName = QFileDialog::getExistingDirectory(this->parentWidget(), "Выберите директорию", QDir::homePath());
    if (_lineEdit != nullptr) {
        _lineEdit->setText(dirName);
    }
}
