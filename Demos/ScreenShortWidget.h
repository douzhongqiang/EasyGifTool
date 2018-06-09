#ifndef SCREENSHORT_WIDGET_H
#define SCREENSHORT_WIDGET_H
#include <QWidget>
#include <QPoint>
#include <QPushButton>
#include <QHBoxLayout>

class ScreenShortWidget : public QWidget
{
    Q_OBJECT

public:
    enum ScreenShortMode
    {
        ScreenShort_None,
        ScreenShort_ScreenCutMode,  // 截取区域的一个位置
        ScreenShort_MoveRectMode,   // 移动当前截取到任意位置
    };

public:
    ScreenShortWidget(QWidget *parent = nullptr);
    ~ScreenShortWidget();

    // 获取截取的矩形框大小
    QRect getRect(void);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPoint m_StartPos;
    QPoint m_EndPos;

    // 当前的工作模式
    ScreenShortMode m_ScreenShortType;
    // 移动的起始坐标点
    QPoint m_MoveStartPos;

    // 工具栏
    void initToolWidget(void);
    QWidget *m_ToolWidget = nullptr;
    QPushButton *m_ConfirmButton = nullptr;     // 确定按钮
    QPushButton *m_CannelButton = nullptr;      // 取消按钮

    QRect m_Rect;
    QRect getCurrentScreenShortRect(void);

signals:
    void signalFinishedScreenShort(QRect);
    void signalCannelScreenShort(void);

private slots:
    void onCannelScreenShort(void);
    void onConfirmScreenShort(void);
};

#endif
