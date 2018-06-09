#include "UIProgressWindow.h"
#include <QVBoxLayout>

UIProgressWindow::UIProgressWindow(QWidget *parent)
    :UIBaseWindow(parent)
{
    m_TagLabelText = new QLabel;
    m_ProgressBar = new QProgressBar;
    m_ProgressBar->setValue(0);
    m_ProgressBar->setRange(0, 100);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(m_ProgressBar);
    mainLayout->addWidget(m_TagLabelText, 0, Qt::AlignLeft);
}

UIProgressWindow::~UIProgressWindow()
{

}

// 设置显示内容
void UIProgressWindow::setTagContent(QString str)
{
    m_TagLabelText->setText(str);
}

// 设置当前的显示状态
void UIProgressWindow::setProgressValue(int value)
{
    m_ProgressBar->setValue(value);
}
