#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include <QWidget>
#include "UIBase/UIBaseWindow.h"

class BezierCurveWidget : public QWidget
{
    Q_OBJECT

public:
    BezierCurveWidget(QWidget *parent = nullptr);
    ~BezierCurveWidget();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint m_StartPos;
    QPoint m_EndPos;
    QPoint m_CtrolPos1;
    QPoint m_CtrolPos2;

    bool onClickedStartPos = false;
    bool onClickedEndPos = false;
    bool onClickedCtrol1Pos = false;
    bool onClickedCtrol2Pos = false;

    // 圆的半径
    int m_RInterval = 8;

    // 绘制起始点和控制点
    void drawPoints(QPainter* painter);
    // 根据点获取包围矩形
    QRect getOutSizeRect(QPoint);
    // 绘制连线
    void drawLines(QPainter* painter);
};

// -------------------------------------------------------------------------------
class BezierCurveWindow : public UIBaseWindow
{
    Q_OBJECT

public:
    BezierCurveWindow(QWidget *parent = nullptr);
    ~BezierCurveWindow();
};

#endif
