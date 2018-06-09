#include "ThreadSigAndSlots.h"
#include "UIBase/UIGlobalTool.h"
#include <QVBoxLayout>
#include <QDebug>

ThreadSigAndSlotsWidget::ThreadSigAndSlotsWidget(QWidget *parent)
    :UIBaseWindow(parent)
{
    m_ProgressBar = new QProgressBar;
    m_StartButton = new QPushButton("Start");
    // 添加阴影
    g_GlobalTool->addShadowEffect(m_StartButton);
    m_ProgressBar->setValue(0);
    m_ProgressBar->setRange(0, 100);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->addSpacerItem(new QSpacerItem(10, 30));
    mainLayout->addWidget(m_ProgressBar, 0, Qt::AlignTop | Qt::AlignVCenter);
    mainLayout->addWidget(m_StartButton, 0, Qt::AlignTop | Qt::AlignVCenter);

    m_Thread = new SignalThread(this);

    // 链接信号和槽
    QObject::connect(m_Thread, &SignalThread::valueChanged, \
                     this, &ThreadSigAndSlotsWidget::recvThreadValue);
    QObject::connect(m_StartButton, &QPushButton::clicked, \
                     this, &ThreadSigAndSlotsWidget::onClickedStarButton);

    m_Thread->start();
}

ThreadSigAndSlotsWidget::~ThreadSigAndSlotsWidget()
{

}

void ThreadSigAndSlotsWidget::recvThreadValue(int value)
{
    m_ProgressBar->setValue(value);
    qDebug() << "GUI Thread ID: " << QThread::currentThreadId();
}

void ThreadSigAndSlotsWidget::onClickedStarButton(void)
{
    if (!m_Thread->isRunning())
        m_Thread->start();
}

// --------------------- Signal Thread -------------------------
SignalThread::SignalThread(QObject *parnet)
    :QThread(parnet)
{

}

SignalThread::~SignalThread()
{

}

void SignalThread::run(void)
{
    while (1)
    {
        qDebug() << "Thread ID: " << QThread::currentThreadId();
        for (int i=0; i<=100; ++i)
        {
            emit valueChanged(i);
            QThread::msleep(100);
        }

        return;
    }
}
