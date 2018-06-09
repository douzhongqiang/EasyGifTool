#include "UIGlobalTool.h"
#include <QGraphicsDropShadowEffect>

UIGlobalTool::UIGlobalTool(QObject *parent)
    :QObject(parent)
{

}

UIGlobalTool::~UIGlobalTool()
{

}

UIGlobalTool *UIGlobalTool::getInstance(void)
{
    static UIGlobalTool instance;
    return &instance;
}

void UIGlobalTool::addShadowEffect(QWidget *shadowWdt)
{
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setOffset(1, 1);
    shadowEffect->setColor(QColor(40, 40, 40));
    shadowEffect->setBlurRadius(2);
    shadowWdt->setGraphicsEffect(shadowEffect);
}
