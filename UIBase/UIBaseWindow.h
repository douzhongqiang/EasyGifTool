#ifndef UIBASE_WINDOW_H
#define UIBASE_WINDOW_H

#include <QWidget>

class UIBaseWindow : public QWidget
{
public:
    UIBaseWindow(QWidget *parent = nullptr);
    ~UIBaseWindow();

    // 设置标题
    void setCustomerWindowTitle(QString titleString);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    int m_nBorder;
    int m_TitleIconSize;
    int m_ButtonIconSize;

    QPixmap *m_TitleIcon = nullptr;
    QPixmap *m_MinSizeIcon = nullptr;
    QPixmap *m_MinSizeHIcon = nullptr;
    QPixmap *m_MaxSizeIcon = nullptr;
    QPixmap *m_MaxSizeHIcon = nullptr;
    QPixmap *m_CloseIcon = nullptr;
    QPixmap *m_CloseHIcon = nullptr;

    QString m_WindowTitle;
    int m_TitleHieght = 30;

    bool m_IsOnMinButton = false;
    bool m_IsOnMaxButton = false;
    bool m_IsOnCloseButton = false;
    bool m_IsOnConfigButton = false;

    // 处理鼠标点击事件
    void disposeMousePressedEvent(void);
    // 处理鼠标移动事件
    void disposeMouseMoveEvent(void);
    // 处理鼠标点击释放事件
    void disposeMouseReleaseEvent(void);

    // 最小化窗口
    void minSizeWindow(void);
    // 最大化窗口
    void maxSizeWindow(void);
    // 关闭窗口
    void closeWindow(void);
};

#endif
