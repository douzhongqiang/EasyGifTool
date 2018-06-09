#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QWidget>
#include <QPixmap>
#include <QMovie>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QTimer>
#include <QPropertyAnimation>

class ImageView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float alpha READ alpha WRITE setAlpha)

public:
    ImageView();
    ~ImageView();

    // 显示图片
    void setPixmap(QPixmap pixmap);
    // 显示Gif图片
    void setGifPic(QString);
    // 显示多个图片
    void setPixmapList(QList<QPixmap> pixmapList);
    // 获取多个图片
    QList<QPixmap> getCurrentPixmapList(void);

private:
    QPixmap m_CurrentPixmap;
    QMovie *m_GifMovie = nullptr;
    QLabel *m_GifLabel = nullptr;
    bool m_IsShowGif = false;

    QWidget *m_ButtonWidget = nullptr;
    QLabel *m_PageTagLabel = nullptr;
    QPushButton *m_PreviousButton = nullptr;
    QPushButton *m_NextButton = nullptr;

    QList<QPixmap> m_PixmapList;
    int m_CurrentIndex = 0;

    // 绘制图片的矩形框
    QRect m_PictureRect;
    // 绘制gif图片的定时器
    QTimer *m_GifTimer = nullptr;
    // 背景图片
    QPixmap m_BackgroundPixmap;
    // 动画
    QPropertyAnimation *m_PropertyAnimation = nullptr;
    float m_PainterAlpha;
    float alpha(void);
    void setAlpha(float alpha);

    // 菜单相关
    QMenu *m_ContextMenu = nullptr;
    void createMenu(void);
    // 更新当前页数显示
    void updateCurrentPage(void);

private slots:
    void onPreviousButtonClicked(void);
    void onNextButtonClicked(void);
    void onCustomContextMenuRequested(QPoint);
    void onDeleteCurrentPixmap(void);
    void onInsertPixmap(void);
    void onTimeout(void);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif
