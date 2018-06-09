#include "UIMessageBox.h"
#include <QVBoxLayout>

UIMessageBox::UIMessageBox(QWidget *parent)
    :UIBaseWindow(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_TagLabel = new QLabel();
    mainLayout->addSpacing(30);
    mainLayout->addWidget(m_TagLabel);
    QPushButton *confirmButton = new QPushButton("确定");
    mainLayout->addWidget(confirmButton, 0, Qt::AlignRight);

    QObject::connect(confirmButton, &QPushButton::clicked, [=](void)->void{
        this->close();
    });
}

UIMessageBox::~UIMessageBox()
{

}

void UIMessageBox::setTagText(QString tagText)
{
    m_TagLabel->setText(tagText);
}

void UIMessageBox::abort(QString title, QString message)
{
    static UIMessageBox messageBox;
    messageBox.setWindowFlags(messageBox.windowFlags() | Qt::WindowStaysOnTopHint);
    messageBox.setCustomerWindowTitle(title);
    messageBox.setTagText(message);
    messageBox.resize(300, 150);
    messageBox.show();
}
