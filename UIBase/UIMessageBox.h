#ifndef UIMESSAGEBOX_H
#define UIMESSAGEBOX_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "UIBaseWindow.h"

class UIMessageBox : public UIBaseWindow
{
    Q_OBJECT

public:
    UIMessageBox(QWidget *parent = nullptr);
    ~UIMessageBox();

private:
    QLabel *m_TagLabel = nullptr;

public:
    void abort(QString title, QString message);
    // 设置文本
    void setTagText(QString);
};

#endif
