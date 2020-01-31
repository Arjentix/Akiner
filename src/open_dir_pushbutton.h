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

#ifndef OPENDIRPUSHBUTTON_H
#define OPENDIRPUSHBUTTON_H

#include <QPushButton>
#include <QLineEdit>

class OpenDirPushButton : public QPushButton
{
Q_OBJECT
public:
    OpenDirPushButton(QWidget* parent = nullptr, QLineEdit* lineEdit = nullptr);
    virtual ~OpenDirPushButton();

private slots:
    void open_dir_dialog();

private:
    // _lineEdit - pointer to the QLineEdit object on which a selected path will appear
    QLineEdit* _lineEdit;
};

#endif // OPENDIRPUSHBUTTON_H
