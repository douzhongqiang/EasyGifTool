#include "BezierCurve.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QMouseEvent>

BezierCurveWidget::BezierCurveWidget(QWidget *parent)
    :QWidget(parent)
{
    m_StartPos = QPoint(100, 100);
    m_EndPos = QPoint(600, 600);
    m_CtrolPos1 = QPoint(100, 600);
    m_CtrolPos2 = QPoint(600, 100);
}

BezierCurveWidget::~BezierCurveWidget()
{

}

void BezierCurveWidget::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.moveTo(m_StartPos);

    // 添加贝塞尔曲线绘制路径
    path.cubicTo(m_CtrolPos1, m_CtrolPos2, m_EndPos);

    // 设置画笔
    QPainter nPainter(this);
    QPen nPen;
    nPen.setColor(QColor(0, 200, 0));
    nPen.setWidth(4);
    nPainter.setPen(nPen);
    // 设置抗锯齿
    nPainter.setRenderHint(QPainter::Antialiasing, true);

    // 绘制路径
    nPainter.drawPath(path);
    // 绘制起始点和控制点
    drawPoints(&nPainter);
    // 绘制连线
    drawLines(&nPainter);

    return QWidget::paintEvent(event);
}

void BezierCurveWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint nMousePos = event->pos();

    if (getOutSizeRect(m_StartPos).contains(nMousePos))
        onClickedStartPos = true;
    else if (getOutSizeRect(m_EndPos).contains(nMousePos))
        onClickedEndPos = true;
    else if (getOutSizeRect(m_CtrolPos1).contains(nMousePos))
        onClickedCtrol1Pos = true;
    else if (getOutSizeRect(m_CtrolPos2).contains(nMousePos))
        onClickedCtrol2Pos = true;

    return QWidget::mousePressEvent(event);
}

void BezierCurveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    onClickedStartPos = false;
    onClickedEndPos = false;
    onClickedCtrol1Pos = false;
    onClickedCtrol2Pos = false;

    return QWidget::mouseReleaseEvent(event);
}

void BezierCurveWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint nMousePos = event->pos();

    if (onClickedStartPos)
        m_StartPos = nMousePos;
    else if (onClickedEndPos)
        m_EndPos = nMousePos;
    else if (onClickedCtrol1Pos)
        m_CtrolPos1 = nMousePos;
    else if (onClickedCtrol2Pos)
        m_CtrolPos2 = nMousePos;
    this->repaint();

    return QWidget::mouseMoveEvent(event);
}

void BezierCurveWidget::drawPoints(QPainter* painter)
{
    painter->save();

    // 红色圆圈绘制起始点
    painter->setPen(QPen(QColor(255, 0, 0)));
    painter->drawEllipse(m_StartPos, m_RInterval, m_RInterval);
    painter->drawEllipse(m_EndPos, m_RInterval, m_RInterval);

    // 蓝色圆圈绘制控制点
    painter->setPen(QPen(QColor(0, 255, 255)));
    painter->drawEllipse(m_CtrolPos1, m_RInterval, m_RInterval);
    painter->drawEllipse(m_CtrolPos2, m_RInterval, m_RInterval);

    painter->restore();
}

void BezierCurveWidget::drawLines(QPainter* painter)
{
    painter->save();

    // 绘制控制点与起始点的连线
    QPen nPen;
    nPen.setStyle(Qt::DotLine);
    nPen.setColor(QColor(200, 100, 100));
    nPen.setWidth(1);
    painter->setPen(nPen);

    painter->drawLine(m_StartPos, m_CtrolPos1);
    painter->drawLine(m_EndPos, m_CtrolPos2);
    painter->drawLine(m_CtrolPos1, m_CtrolPos2);

    painter->restore();
}

QRect BezierCurveWidget::getOutSizeRect(QPoint pos)
{
    int interval = m_RInterval;
    return QRect(pos.x() - interval, pos.y() - interval, 2 * interval, 2 * interval);
}

// -------------------------------------------------------------------------------
BezierCurveWindow::BezierCurveWindow(QWidget *parent)
    :UIBaseWindow(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addSpacerItem(new QSpacerItem(10, 30));
    mainLayout->addWidget(new BezierCurveWidget);
}

BezierCurveWindow::~BezierCurveWindow()
{

}
