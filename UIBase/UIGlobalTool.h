#ifndef UIGLOBALTOOL_H
#define UIGLOBALTOOL_H

#include <QObject>
#include <QWidget>
#define g_GlobalTool UIGlobalTool::getInstance()

class UIGlobalTool : public QObject
{
    Q_OBJECT

private:
    UIGlobalTool(QObject *parent = nullptr);
    ~UIGlobalTool();

public:
    static UIGlobalTool *getInstance(void);

    // 添加阴影
    void addShadowEffect(QWidget *shadowWdt);
};

#endif
