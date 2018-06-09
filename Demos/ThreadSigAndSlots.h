#ifndef THREAD_SIGANDSLOT_H
#define THREAD_SIGANDSLOT_H

#include <QProgressBar>
#include <QWidget>
#include <QPushButton>
#include <QThread>
#include "UIBase/UIBaseWindow.h"

class SignalThread;
class ThreadSigAndSlotsWidget : public UIBaseWindow
{
    Q_OBJECT

public:
    ThreadSigAndSlotsWidget(QWidget *parent = nullptr);
    ~ThreadSigAndSlotsWidget();

private:
    QProgressBar *m_ProgressBar = nullptr;
    QPushButton *m_StartButton = nullptr;

    SignalThread *m_Thread = nullptr;

private slots:
    void recvThreadValue(int);
    void onClickedStarButton(void);
};

// --------------------- Signal Thread -------------------------
class SignalThread : public QThread
{
    Q_OBJECT

public:
    SignalThread(QObject *parnet = nullptr);
    ~SignalThread();

protected:
    virtual void run(void) override;

signals:
    void valueChanged(int);
};

#endif
