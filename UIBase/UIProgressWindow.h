#ifndef UIPROGRESS_WINDOW_H
#define UIPROGRESS_WINDOW_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include "UIBaseWindow.h"

class UIProgressWindow : public UIBaseWindow
{
    Q_OBJECT

public:
    UIProgressWindow(QWidget *parent = nullptr);
    ~UIProgressWindow();

    // 设置显示内容
    void setTagContent(QString str);
    // 设置当前的显示状态
    void setProgressValue(int);

private:
    QLabel *m_TagLabelText = nullptr;
    QProgressBar *m_ProgressBar = nullptr;
};

#endif
