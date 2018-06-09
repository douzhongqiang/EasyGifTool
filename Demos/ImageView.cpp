#include "ImageView.h"
#include <QPixmap>
#include <QPainter>
#include <QVBoxLayout>
#include <QApplication>
#include <QStyleOption>
#include <QFileDialog>
#include "UIBase/UIGlobalTool.h"

ImageView::ImageView()
{
    m_PixmapList.clear();
    m_GifLabel = new QLabel();
    QVBoxLayout *layout = new QVBoxLayout;
    layout ->addWidget(m_GifLabel);

    layout->setSpacing(0);
    layout->setMargin(0);
    this->setLayout(layout);
    m_GifLabel->hide();

    m_ButtonWidget = new QWidget;
    m_ButtonWidget->setFixedHeight(50);
    QHBoxLayout *m_Layout = new QHBoxLayout(m_ButtonWidget);
    m_PreviousButton = new QPushButton("上一张");
    m_NextButton = new QPushButton("下一张");
    m_PreviousButton->setEnabled(false);
    m_NextButton->setEnabled(false);
    g_GlobalTool->addShadowEffect(m_PreviousButton);
    g_GlobalTool->addShadowEffect(m_NextButton);
    m_PageTagLabel = new QLabel("");
    m_Layout->addWidget(m_PreviousButton);
    m_Layout->addWidget(m_NextButton);
    m_Layout->addWidget(m_PageTagLabel);
    m_Layout->addStretch();

    layout->addWidget(m_ButtonWidget, 0, Qt::AlignBottom);

    this->setProperty("insetWidget", "insetWidget");

    createMenu();
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    // 定时器
    m_GifTimer = new QTimer(this);
    m_PainterAlpha = 1.0;

    // 动画
    m_PropertyAnimation = new QPropertyAnimation(this, "alpha", this);
    m_PropertyAnimation->setStartValue(0.2);
    m_PropertyAnimation->setEndValue(1.0);
    m_PropertyAnimation->setDuration(1200);

    // 连接信号和槽
    QObject::connect(m_PreviousButton, SIGNAL(clicked()), this, SLOT(onPreviousButtonClicked()));
    QObject::connect(m_NextButton, SIGNAL(clicked()), this, SLOT(onNextButtonClicked()));
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)), \
                     this, SLOT(onCustomContextMenuRequested(QPoint)));
    QObject::connect(m_GifTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

ImageView::~ImageView()
{

}

float ImageView::alpha(void)
{
    return m_PainterAlpha;
}

void ImageView::setAlpha(float alpha)
{
    m_PainterAlpha = alpha;
    this->repaint();
}

void ImageView::setPixmap(QPixmap pixmap)
{
    m_CurrentPixmap = pixmap;
    m_IsShowGif = false;
    m_PropertyAnimation->stop();
    m_GifTimer->stop();

    m_GifLabel->hide();
    this->repaint();
}

void ImageView::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    int rectHeight = this->height() - 50;
    m_PictureRect.setX(5);
    m_PictureRect.setY(5);
    m_PictureRect.setWidth(this->width() - 10);
    m_PictureRect.setHeight(rectHeight - 10);

    if (!m_CurrentPixmap.isNull())
    {
        QImage image = m_CurrentPixmap.toImage();

        int imageWidth = image.width() > m_PictureRect.width() ? m_PictureRect.width() : image.width();
        int imageHeight = imageWidth / (image.width() * 1.0 / image.height());
        imageHeight = imageHeight > m_PictureRect.height() ? m_PictureRect.height() : imageHeight;
        imageWidth = imageHeight * (image.width() * 1.0 / image.height());

        // 绘制图片
        int xPt = m_PictureRect.x() + (m_PictureRect.width() - imageWidth) * 1.0 / 2;
        int yPt = m_PictureRect.y() + (m_PictureRect.height() - imageHeight) * 1.0 / 2;

        // 绘制背景图片
        if (!m_BackgroundPixmap.isNull())
        {
            painter.setOpacity(1.0 - m_PainterAlpha);
            painter.drawPixmap(QRect(xPt, yPt, imageWidth, imageHeight), m_BackgroundPixmap);
        }

        painter.setOpacity(m_PainterAlpha);
        painter.drawImage(QRect(xPt, yPt, imageWidth, imageHeight), image);
    }
    else
    {
        QTextOption o;
        o.setAlignment(Qt::AlignCenter);
        painter.setPen(QColor(255, 255, 255));
        painter.drawText(this->rect(), "没有加载/生成图像", o);
    }

    return QWidget::paintEvent(event);
}

void ImageView::setGifPic(QString filePath)
{
    if (m_GifMovie)
        delete m_GifMovie;
    m_GifMovie = new QMovie(filePath);

    // Gif
    m_GifMovie->jumpToFrame(0);
    QPixmap pixmap = m_GifMovie->currentPixmap();
    m_CurrentPixmap = pixmap;
    int delayTimes = m_GifMovie->nextFrameDelay();
    m_GifTimer->setInterval(delayTimes);
    m_GifTimer->start();

    // 按钮失效
    m_PreviousButton->setEnabled(false);
    m_NextButton->setEnabled(false);
    m_PageTagLabel->setText("");
    this->repaint();
}

void ImageView::setPixmapList(QList<QPixmap> pixmapList)
{
    m_PixmapList = pixmapList;
    if (m_PixmapList.count() > 0)
    {
        m_GifTimer->stop();
        m_GifLabel->hide();
        m_IsShowGif = false;
        m_CurrentPixmap = m_PixmapList.at(0);
        m_CurrentIndex = 0;
        m_PropertyAnimation->start();
        updateCurrentPage();
        this->repaint();

        // 按钮生效
        m_PreviousButton->setEnabled(true);
        m_NextButton->setEnabled(true);
    }
}

QList<QPixmap> ImageView::getCurrentPixmapList(void)
{
    return m_PixmapList;
}

void ImageView::onPreviousButtonClicked(void)
{
    if (m_PixmapList.isEmpty())
        return;

    if (m_CurrentIndex == 0)
        return;

    m_CurrentIndex--;
    if (m_CurrentIndex < 0)
        m_CurrentIndex = 0;

    m_BackgroundPixmap = m_CurrentPixmap;
    m_PropertyAnimation->stop();
    m_PropertyAnimation->start();

    m_CurrentPixmap = m_PixmapList.at(m_CurrentIndex);
    updateCurrentPage();
    this->repaint();
}

void ImageView::onNextButtonClicked(void)
{
    if (m_PixmapList.isEmpty())
        return;

    if (m_CurrentIndex == m_PixmapList.count() - 1)
        return;

    m_CurrentIndex++;
    if (m_PixmapList.count() <= m_CurrentIndex)
        m_CurrentIndex = m_PixmapList.count() - 1;

    m_BackgroundPixmap = m_CurrentPixmap;
    m_PropertyAnimation->stop();
    m_PropertyAnimation->start();

    m_CurrentPixmap = m_PixmapList.at(m_CurrentIndex);
    updateCurrentPage();
    this->repaint();
}

void ImageView::onCustomContextMenuRequested(QPoint pos)
{
    if (m_ContextMenu)
    {
        QPoint nMenuPos = this->mapToGlobal(pos);
        m_ContextMenu->exec(nMenuPos);
    }
}

void ImageView::updateCurrentPage(void)
{
    if (m_PixmapList.count() <= 0)
        return;

    QString nPageString = "共%1页，第%2页";
    nPageString = nPageString.arg(m_PixmapList.count()).arg(m_CurrentIndex + 1);
    m_PageTagLabel->setText(nPageString);
}

void ImageView::createMenu(void)
{
    m_ContextMenu = new QMenu(this);
    QAction *nPreviousAction = new QAction("上一张", m_ContextMenu);
    QAction *nNextAction = new QAction("下一张", m_ContextMenu);
    QAction *nDeleteAction = new QAction("删除图片", m_ContextMenu);
    QAction *nInsertAction = new QAction("插入图片", m_ContextMenu);

    m_ContextMenu->addAction(nPreviousAction);
    m_ContextMenu->addAction(nNextAction);
    m_ContextMenu->addSeparator();
    m_ContextMenu->addAction(nDeleteAction);
    m_ContextMenu->addAction(nInsertAction);

    // 连接信号和槽
    QObject::connect(nPreviousAction, SIGNAL(triggered(bool)), this, SLOT(onPreviousButtonClicked()));
    QObject::connect(nNextAction, SIGNAL(triggered(bool)), this, SLOT(onNextButtonClicked()));
    QObject::connect(nDeleteAction, SIGNAL(triggered(bool)), this, SLOT(onDeleteCurrentPixmap()));
    QObject::connect(nInsertAction, SIGNAL(triggered(bool)), this, SLOT(onInsertPixmap()));
}

void ImageView::onDeleteCurrentPixmap(void)
{
    if (m_PixmapList.count() <= 0)
        return;

    m_PixmapList.removeAt(m_CurrentIndex--);
    if (m_CurrentIndex < 0)
        m_CurrentIndex = 0;

    if (m_PixmapList.count() > 0)
        m_CurrentPixmap = m_PixmapList.at(m_CurrentIndex);

    updateCurrentPage();
    this->repaint();
}

void ImageView::onInsertPixmap(void)
{
    if (m_IsShowGif)
        return;

    QString fileName = QFileDialog::getOpenFileName(this, "选择一个图片", "./", tr("Image Files (*.png *jpg *bmp)"));
    if (fileName.isEmpty())
        return;

    QPixmap pixmap;
    if (!pixmap.load(fileName))
        return;

    m_PixmapList.insert(m_CurrentIndex, pixmap);
    m_CurrentIndex++;
    m_CurrentPixmap = pixmap;
    updateCurrentPage();
    this->repaint();
}

void ImageView::onTimeout(void)
{
    if (!m_GifMovie)
        return;

    m_GifMovie->jumpToNextFrame();
    m_CurrentPixmap = m_GifMovie->currentPixmap();
    int delaySleep = m_GifMovie->nextFrameDelay();
    m_GifTimer->setInterval(delaySleep);
    this->repaint();
}
