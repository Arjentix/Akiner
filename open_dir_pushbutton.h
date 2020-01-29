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
