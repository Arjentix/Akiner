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

#include "open_dir_pushbutton.h"

#include <QFileDialog>
#include <QDir>

OpenDirPushButton::OpenDirPushButton(QWidget* parent, QLineEdit* lineEdit)
    : QPushButton(parent)
    , _lineEdit(lineEdit)
{
    QPixmap pixmap(":/icons/open_dir.jpeg");
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
