#include "GifCreater.h"

GifCreater::GifCreater(QObject *parent)
    :QThread(parent)
{

}

GifCreater::~GifCreater()
{

}

GifCreater* GifCreater::getInstance(void)
{
    static GifCreater nGifCreater;
    return &nGifCreater;
}

// 设置gif图片的尺寸
void GifCreater::setGifPicSize(int w, int h)
{
    m_Width = w;
    m_Height = h;
}

// 添加图片列表
void GifCreater::addPicList(QPixmap pixmap)
{
    QMutexLocker locker(&m_Mutex);
    m_PixmapList.push_back(pixmap);
}

void GifCreater::startGifCreater(QString pathFileString)
{
    if (m_GifWriter)
        delete m_GifWriter;

    m_GifWriter = new Gif_H::GifWriter;
    if (!m_Gif.GifBegin(m_GifWriter, pathFileString.toLocal8Bit().data(), m_Width, m_Height, m_Fps))
    {
        delete m_GifWriter;
        m_GifWriter = nullptr;
        return;
    }

    m_StartThread = true;
}

void GifCreater::endGifCreater(void)
{
    // 外部线程调用
    while (1)
    {
        bool isBreak = false;
        m_Mutex.lock();
        isBreak = m_PixmapList.isEmpty() ? true : false;
        m_Mutex.unlock();

        if (!isBreak)
            QThread::msleep(100);
        else
            break;
    }

    m_Gif.GifEnd(m_GifWriter);

    delete m_GifWriter;
    m_GifWriter = nullptr;
}

void GifCreater::endGifCreater2(void)
{
    m_Gif.GifEnd(m_GifWriter);

    delete m_GifWriter;
    m_GifWriter = nullptr;
}

void GifCreater::setSpeed(int speed)
{
    QMutexLocker locker(&m_Mutex);
    m_Fps = speed;
}

void GifCreater::run(void)
{
    while (!this->isInterruptionRequested())
    {
        if (!m_PixmapList.isEmpty())
        {
            QMutexLocker locker(&m_Mutex);
            QPixmap pixmap = m_PixmapList.at(0);
            QImage image = pixmap.toImage();
            image = image.convertToFormat(QImage::Format_RGBA8888);
            m_PixmapList.pop_front();

            // 写入图片信息
            m_Gif.GifWriteFrame(m_GifWriter, image.bits(), image.width(), image.height(), m_Fps);
            emit signalCurrentSubPage(m_PixmapList.count());
        }
        else
            QThread::msleep(100);
    }
}

void GifCreater::testGif(void)
{
    QImage img1 = QImage("1_1_1.jpg");
    img1 = img1.convertToFormat(QImage::Format_RGBA8888);
    QImage img2 = QImage("2_2_2.jpg");
    img2 = img2.convertToFormat(QImage::Format_RGBA8888);
    QImage img3 = QImage("3_3_3.jpg");
    img3 = img3.convertToFormat(QImage::Format_RGBA8888);

    Gif_H gifH;

    Gif_H::GifWriter writer;
    if (!gifH.GifBegin(&writer, "123.gif", img1.size().width(), \
                       img1.size().height(), 50))
        return;

    gifH.GifWriteFrame(&writer, img1.constBits(), img1.width(), img1.height(), 50);
    gifH.GifWriteFrame(&writer, img2.constBits(), img2.width(), img2.height(), 50);
    gifH.GifWriteFrame(&writer, img3.constBits(), img3.width(), img3.height(), 50);

    gifH.GifEnd(&writer);
    qDebug() << "Over!";
}
