#include "ScreenShortWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDesktopWidget>

ScreenShortWidget::ScreenShortWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(true);

    QDesktopWidget desktopWdt;
    int width = desktopWdt.width();
    int height = desktopWdt.height();
    this->setGeometry(0, 0, width, height);

    m_StartPos = QPoint(0, 0);
    m_EndPos = QPoint(0, 0);

    m_ScreenShortType = ScreenShort_None;

    initToolWidget();
}

ScreenShortWidget::~ScreenShortWidget()
{

}

QRect ScreenShortWidget::getRect(void)
{
    return m_Rect;
}

QRect ScreenShortWidget::getCurrentScreenShortRect(void)
{
    return QRect(m_StartPos, m_EndPos);
}

void ScreenShortWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(103, 161, 37, 2));

    QPainterPath path;
    if (m_StartPos != m_EndPos)
        path.addRect(QRect(m_StartPos, m_EndPos));

    path.addRect(this->rect());

    painter.setPen(QColor(60, 60, 60));
    painter.setBrush(QBrush(QColor(103, 161, 37, 100)));
    painter.drawPath(path);

    return QWidget::paintEvent(event);
}

void ScreenShortWidget::mousePressEvent(QMouseEvent *event)
{
    // 获取当前坐标点
    QPoint pos = event->pos();

    if (ScreenShort_None == m_ScreenShortType)
    {
        // 截取区域模式
        m_StartPos = pos;
        // 进入截取模式
        m_ScreenShortType = ScreenShort_ScreenCutMode;
    }
    else if (m_ScreenShortType == ScreenShort_ScreenCutMode || \
             m_ScreenShortType == ScreenShort_MoveRectMode)
    {
        // 若鼠标点击的点在当前矩形框内，则可移动；
        // 否则重新绘制选择区域
        if (getCurrentScreenShortRect().contains(pos))
        {
            // 设置起始移动位置
            m_MoveStartPos = event->pos();
            // 进入截取区域移动模式
            m_ScreenShortType = ScreenShort_MoveRectMode;
        }
        else
        {
            m_StartPos = pos;
            m_EndPos = pos;
            m_ScreenShortType = ScreenShort_ScreenCutMode;
        }
    }

    return QWidget::mousePressEvent(event);
}

void ScreenShortWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // 截取区域模式
    if (m_ScreenShortType == ScreenShort_ScreenCutMode)
    {
        m_EndPos = event->pos();
        this->repaint();
    }

    return QWidget::mouseReleaseEvent(event);
}

void ScreenShortWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    // 截取区域模式
    if (m_ScreenShortType == ScreenShort_ScreenCutMode)
    {
        m_EndPos = pos;
        this->repaint();
    }
    else if (m_ScreenShortType == ScreenShort_MoveRectMode)
    {
        int xInterval = pos.x() - m_MoveStartPos.x();
        int yInterval = pos.y() - m_MoveStartPos.y();

        // 设置矩形起始点位置
        m_StartPos.setX(m_StartPos.x() + xInterval);
        m_StartPos.setY(m_StartPos.y() + yInterval);

        // 设置矩形终止点的位置
        m_EndPos.setX(m_EndPos.x() + xInterval);
        m_EndPos.setY(m_EndPos.y() + yInterval);

        m_MoveStartPos = pos;
        this->repaint();
    }

    return QWidget::mouseMoveEvent(event);
}

void ScreenShortWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        m_ScreenShortType = ScreenShort_None;
        m_StartPos = QPoint(0, 0);
        m_EndPos = m_StartPos;
        this->repaint();
    }

    return QWidget::keyPressEvent(event);
}

void ScreenShortWidget::initToolWidget(void)
{
    QWidget *m_ToolWidget = new QWidget(this);
    m_ToolWidget->setProperty("NormalWidget", "NormalWidget");
    QHBoxLayout *layout = new QHBoxLayout(m_ToolWidget);

    m_ConfirmButton = new QPushButton;     // 确定按钮
    m_ConfirmButton->setFixedSize(30, 30);
    m_ConfirmButton->setIcon(QIcon("./Icons/Confirm.png"));
    m_CannelButton = new QPushButton;      // 取消按钮
    m_CannelButton->setFixedSize(30, 30);
    m_CannelButton->setIcon(QIcon("./Icons/Cannel.png"));

    QObject::connect(m_ConfirmButton, SIGNAL(clicked()), this, SLOT(onConfirmScreenShort()));
    QObject::connect(m_CannelButton, SIGNAL(clicked()), this, SLOT(onCannelScreenShort()));

    layout->setMargin(1);
    layout->setSpacing(1);
    layout->addStretch();
    layout->addWidget(m_CannelButton);
    layout->addWidget(m_ConfirmButton);


    m_ToolWidget->setGeometry(this->width() - 200 - 5, this->height() - 40 - 10, 200, 40);
}

void ScreenShortWidget::onCannelScreenShort(void)
{
    this->hide();
    m_ScreenShortType = ScreenShort_None;
    m_StartPos = QPoint(0, 0);
    m_EndPos = m_StartPos;
    emit signalCannelScreenShort();
}

void ScreenShortWidget::onConfirmScreenShort(void)
{
    this->hide();

    // 发送截图结束信号
    if (m_ScreenShortType != ScreenShort_None)
        emit signalFinishedScreenShort(QRect(m_StartPos, m_EndPos));
    m_Rect = getCurrentScreenShortRect();

    m_ScreenShortType = ScreenShort_None;
    m_StartPos = QPoint(0, 0);
    m_EndPos = m_StartPos;
}
