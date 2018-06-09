#ifndef GIFCREATER_H
#define GIFCREATER_H
#include <QString>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "ToolBase/gif_h/gif.h"

#define g_GifCreater GifCreater::getInstance()

class GifCreater : public QThread
{
    Q_OBJECT
private:
    GifCreater(QObject *parent = nullptr);
    ~GifCreater();

public:
    static GifCreater* getInstance(void);

    // 设置gif图片的尺寸
    void setGifPicSize(int w, int h);
    // 添加图片列表
    void addPicList(QPixmap pixmap);

    // 开始线程
    void startGifCreater(QString pathFileString);
    // 结束线程
    void endGifCreater(void);
    void endGifCreater2(void);
    // 设置录制的速度
    void setSpeed(int);

    void run(void) override;

    // 测试Gif
    void testGif(void);

private:
    int m_Width = 20;
    int m_Height = 20;
    QMutex m_Mutex;

    Gif_H m_Gif;
    Gif_H::GifWriter *m_GifWriter = nullptr;
    int m_Fps = 10; // 100为1s

    // 图像缓存队列
    QList<QPixmap> m_PixmapList;
    // 线程状态控制
    bool m_StartThread = true;

signals:
    // 合成完成一张信号，剩余的张数
    void signalCurrentSubPage(int);
};

#endif
