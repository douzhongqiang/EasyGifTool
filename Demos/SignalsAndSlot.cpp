#include "SignalsAndSlot.h"
#include "UIBase/UIGlobalTool.h"

SignalsAndSlotDemo::SignalsAndSlotDemo(QWidget *parent)
    :UIBaseWindow(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_ValueEdit = new QLineEdit("0");
    m_ValueEdit->setReadOnly(true);
    m_AddButton = new QPushButton(tr("Add 1"));
    m_SubButton = new QPushButton(tr("Sub 1"));

    // 添加按钮阴影
    g_GlobalTool->addShadowEffect(m_AddButton);
    g_GlobalTool->addShadowEffect(m_SubButton);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_AddButton);
    bottomLayout->addWidget(m_SubButton);

    mainLayout->addSpacerItem(new QSpacerItem(10, 30));
    mainLayout->addWidget(m_ValueEdit);
    mainLayout->addLayout(bottomLayout);

    // 链接信号和槽函数
    QObject::connect(m_AddButton, SIGNAL(clicked()), this, SLOT(onClickedAddButton()));
    QObject::connect(m_SubButton, &QPushButton::clicked, this, &SignalsAndSlotDemo::onClickedSubButton);
    QObject::connect(this, &SignalsAndSlotDemo::subSignals, [=](void)->void{
        onRecvSubSignals();
    });
}

SignalsAndSlotDemo::~SignalsAndSlotDemo()
{

}

void SignalsAndSlotDemo::onClickedAddButton(void)
{
    int value = m_ValueEdit->text().toInt();
    QString valueString = QString::number(++value);
    m_ValueEdit->setText(valueString);
}

void SignalsAndSlotDemo::onClickedSubButton(void)
{
    emit subSignals();
}

void SignalsAndSlotDemo::onRecvSubSignals(void)
{
    int value = m_ValueEdit->text().toInt();
    QString valueString = QString::number(--value);
    m_ValueEdit->setText(valueString);
}
