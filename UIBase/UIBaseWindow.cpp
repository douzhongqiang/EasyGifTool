#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include "UIBaseWindow.h"
#include <QPainter>
#include <QDebug>
#include <QApplication>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#include <Windowsx.h>
#endif

UIBaseWindow::UIBaseWindow(QWidget *parent)
    :QWidget(parent)
{
    this->installEventFilter(this);
    setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    this->setProperty("NormalWidget", "NormalWidget");

    m_nBorder = 2;
    m_TitleIconSize = 25;
    m_ButtonIconSize = 20;

    m_WindowTitle = "Douzhq Qt Demo";
    m_TitleIcon = new QPixmap("./Icons/AppIconD.png");
    m_MinSizeIcon = new QPixmap("./Icons/MinButton.png");
    m_MinSizeHIcon = new QPixmap("./Icons/minButton_H.png");
    m_MaxSizeIcon = new QPixmap("./Icons/MaxSizeed.png");
    m_MaxSizeHIcon = new QPixmap("./Icons/MaxSizeDemo_H.png");
    m_CloseIcon = new QPixmap("./Icons/Close.png");
    m_CloseHIcon = new QPixmap("./Icons/Close_H.png");
}

UIBaseWindow::~UIBaseWindow()
{

}

void UIBaseWindow::setCustomerWindowTitle(QString titleString)
{
    m_WindowTitle = titleString;
}

bool UIBaseWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

        // 如果鼠标位于子控件上，则不进行处理
        if (childAt(nX, nY) != NULL)
            return QWidget::nativeEvent(eventType, message, result);

        *result = HTCAPTION;

        // 鼠标区域位于窗体边框，进行缩放
        if ((nX > 0) && (nX < m_nBorder))
            *result = HTLEFT;

        if ((nX > this->width() - m_nBorder) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < m_nBorder))
            *result = HTTOP;

        if ((nY > this->height() - m_nBorder) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < m_nBorder) && (nY > 0)
                && (nY < m_nBorder))
            *result = HTTOPLEFT;

        if ((nX > this->width() - m_nBorder) && (nX < this->width())
                && (nY > 0) && (nY < m_nBorder))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < m_nBorder)
                && (nY > this->height() - m_nBorder) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - m_nBorder) && (nX < this->width())
                && (nY > this->height() - m_nBorder) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

void UIBaseWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPen nPen;
    nPen.setColor(QColor(40, 40, 40));
    nPen.setWidth(10);
    painter.setPen(nPen);
    painter.drawRect(0, 0, this->width(), this->height());

    // 绘制标题栏
    painter.setBrush(QColor(40, 40, 40));
    QRect nTitleRect = QRect(0, 0, this->width(), m_TitleHieght);
    painter.drawRect(nTitleRect);

    // 绘制AppIcon
    painter.drawPixmap(5, 5, m_TitleIconSize, m_TitleIconSize, *m_TitleIcon);

    // 绘制按钮
    if (!m_IsOnMinButton)
        painter.drawPixmap(this->width() - (m_TitleIconSize + 4) * 3, 5, m_TitleIconSize, m_TitleIconSize, *m_MinSizeIcon);
    else
        painter.drawPixmap(this->width() - (m_TitleIconSize + 4) * 3, 5, m_TitleIconSize, m_TitleIconSize, *m_MinSizeHIcon);

    if (!m_IsOnMaxButton)
        painter.drawPixmap(this->width() - (m_TitleIconSize + 4) * 2, 5, m_TitleIconSize, m_TitleIconSize, *m_MaxSizeIcon);
    else
        painter.drawPixmap(this->width() - (m_TitleIconSize + 4) * 2, 5, m_TitleIconSize, m_TitleIconSize, *m_MaxSizeHIcon);

    if (!m_IsOnCloseButton)
        painter.drawPixmap(this->width() - (m_TitleIconSize + 4) * 1, 5, m_TitleIconSize, m_TitleIconSize, *m_CloseIcon);
    else
        painter.drawPixmap(this->width() - (m_TitleIconSize + 4) * 1, 5, m_TitleIconSize, m_TitleIconSize, *m_CloseHIcon);

    // 绘制标题
    nPen.setColor(QColor(255, 255, 255));
    painter.setPen(nPen);
    QTextOption o;
    o.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    nTitleRect.setX(5 + m_TitleIconSize + 5);
    painter.drawText(nTitleRect, m_WindowTitle, o);


    return QWidget::paintEvent(event);
}

bool UIBaseWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::NonClientAreaMouseMove)
        disposeMouseMoveEvent();
    else if (event->type() == QEvent::NonClientAreaMouseButtonPress)
        disposeMousePressedEvent();
    else if (event->type() == QEvent::NonClientAreaMouseButtonRelease)
        disposeMouseReleaseEvent();

    return QWidget::eventFilter(obj, event);
}

// 处理鼠标点击事件
void UIBaseWindow::disposeMousePressedEvent(void)
{
    QPoint nPos = cursor().pos();
    nPos = this->mapFromGlobal(nPos);

    if (m_IsOnMinButton)
        minSizeWindow();
    else if (m_IsOnMaxButton)
        maxSizeWindow();
    else if (m_IsOnCloseButton)
        closeWindow();

    disposeMouseReleaseEvent();
}

void UIBaseWindow::minSizeWindow(void)
{
    this->showMinimized();
}

void UIBaseWindow::maxSizeWindow(void)
{
    if (this->isMaximized())
        this->showNormal();
    else
        this->showMaximized();
}

void UIBaseWindow::closeWindow(void)
{
    this->close();
}

// 处理鼠标移动事件
void UIBaseWindow::disposeMouseMoveEvent(void)
{
    QPoint nMousePos = cursor().pos();
    nMousePos = this->mapFromGlobal(nMousePos);

    m_IsOnMinButton = false;
    m_IsOnMaxButton = false;
    m_IsOnCloseButton = false;

    QRect nMinSizeButtonRect = QRect(this->width() - (m_TitleIconSize + 4) * 3, 5, \
                                     m_TitleIconSize, m_TitleIconSize);
    QRect nMaxSizeButtonRect = QRect(this->width() - (m_TitleIconSize + 4) * 2, 5, \
                                     m_TitleIconSize, m_TitleIconSize);
    QRect nCloseButtonRect = QRect(this->width() - (m_TitleIconSize + 4) * 1, 5, \
                                   m_TitleIconSize, m_TitleIconSize);

    if (nMinSizeButtonRect.contains(nMousePos))
        m_IsOnMinButton = true;
    else if (nMaxSizeButtonRect.contains(nMousePos))
        m_IsOnMaxButton = true;
    else if (nCloseButtonRect.contains(nMousePos))
        m_IsOnCloseButton = true;

    this->repaint();
}

void UIBaseWindow::disposeMouseReleaseEvent(void)
{
    m_IsOnMinButton = false;
    m_IsOnMaxButton = false;
    m_IsOnCloseButton = false;
    this->repaint();
}
