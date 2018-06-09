#ifndef SIGNALSANDSLOT_H
#define SIGNALSANDSLOT_H
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include "UIBase/UIBaseWindow.h"

class SignalsAndSlotDemo : public UIBaseWindow
{
    Q_OBJECT
public:
    SignalsAndSlotDemo(QWidget *parent = nullptr);
    ~SignalsAndSlotDemo();

private:
    QLineEdit *m_ValueEdit = nullptr;
    QPushButton *m_AddButton = nullptr;
    QPushButton *m_SubButton = nullptr;

signals:
    void subSignals(void);

private slots:
    void onClickedAddButton(void);
    void onClickedSubButton(void);

private:
    void onRecvSubSignals(void);
};

#endif
