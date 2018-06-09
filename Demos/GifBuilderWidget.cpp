#include "GifBuilderWidget.h"
#include "UIBase/UIGlobalTool.h"
#include <QApplication>
#include <QScreen>
#include <QDateTime>
#include <QFileDialog>
#include <QListView>
#include <QLineEdit>
#include <QAction>
#include <QMessageBox>
#include "UIBase/UIMessageBox.h"
#include "GifCreater.h"

GifBuilderWidget::GifBuilderWidget(QWidget *parent)
    :UIBaseWindow(parent)
{
    m_MainLayout = new QVBoxLayout(this);
    m_MainLayout->addSpacerItem(new QSpacerItem(10, 30));
    this->setCustomerWindowTitle("Easy Gif Tool V1.0 -- by Douzhq");

    m_ImageView = new ImageView;

    QHBoxLayout *layout = new QHBoxLayout;
    m_RightWidget = new QWidget;
    m_RightWidget->setMinimumWidth(250);
    layout->addWidget(m_ImageView, 4);
    layout->addWidget(m_RightWidget, 1);

    // 创建右侧布局
    QVBoxLayout *rightLayout = new QVBoxLayout;
    m_RightWidget->setLayout(rightLayout);

    m_ScreenShortWidget = new ScreenShortWidget;
    m_ScreenShortWidget->hide();

    // 添加截图按钮
    m_ScreenShot = new QPushButton(tr("屏幕截图"));
    g_GlobalTool->addShadowEffect(m_ScreenShot);
    rightLayout->addWidget(m_ScreenShot);
    rightLayout->addSpacing(10);

    // 开始录制Gif按钮
    m_GifStartButton = new QPushButton(tr("开始录制Gif"));
    g_GlobalTool->addShadowEffect(m_GifStartButton);
    rightLayout->addWidget(m_GifStartButton);

    // 结束录制Gif按钮
    m_GifEndButton = new QPushButton(tr("结束录制Gif"));
    m_GifEndButton->setEnabled(false);
    g_GlobalTool->addShadowEffect(m_GifEndButton);
    rightLayout->addWidget(m_GifEndButton);
    rightLayout->addSpacing(10);

    // 导入Gif文件
    m_ImportGifButton = new QPushButton("导入Gif图片");
    g_GlobalTool->addShadowEffect(m_ImportGifButton);
    rightLayout->addWidget(m_ImportGifButton);

    // 分解Gif图片按钮
    m_GifResolveButton = new QPushButton(tr("分解Gif图片"));
    m_GifResolveButton->setEnabled(false);
    g_GlobalTool->addShadowEffect(m_GifResolveButton);
    rightLayout->addWidget(m_GifResolveButton);
    rightLayout->addSpacing(10);

    // 导入多文件
    m_ImportOtherPicButton = new QPushButton(tr("导入多文件"));
    g_GlobalTool->addShadowEffect(m_ImportOtherPicButton);
    rightLayout->addWidget(m_ImportOtherPicButton);

    // 多文件合成Gif
    m_CreateGifButton = new QPushButton(tr("合成Gif"));
    m_CreateGifButton->setEnabled(false);
    g_GlobalTool->addShadowEffect(m_CreateGifButton);
    rightLayout->addWidget(m_CreateGifButton);

    m_Timer = new QTimer(this);
    m_Timer->setInterval(1000 / (100.0 / m_GifCreateSpeed));

    rightLayout->addStretch();

    // 截图时是否隐藏本软件
    QLabel *hideTagLabel = new QLabel("截图/录屏时是否隐藏主窗口：");
    m_ScreenShortHideThis = new QComboBox;
    m_ScreenShortHideThis->setView(new QListView);
    g_GlobalTool->addShadowEffect(m_ScreenShortHideThis);
    m_ScreenShortHideThis->addItem("否");
    m_ScreenShortHideThis->addItem("是");
    bool isHide = g_UIGlobalConfig->getScreenShortMainWidgetShow();
    rightLayout->addWidget(hideTagLabel);
    rightLayout->addWidget(m_ScreenShortHideThis);

    // 创建速度Tag和菜单
    QLabel *speedTagLabel = new QLabel("生成gif文件的间隔速率：");
    m_GifSpeedComboBox = new QComboBox;
    g_GlobalTool->addShadowEffect(m_GifSpeedComboBox);
    m_GifSpeedComboBox->setView(new QListView(m_GifSpeedComboBox));
    m_GifSpeedComboBox->addItem("1Frame/s");
    m_GifSpeedComboBox->addItem("2Frame/s");
    m_GifSpeedComboBox->addItem("5Frame/s");
    m_GifSpeedComboBox->addItem("10Frame/s");
    int index = g_UIGlobalConfig->getScreenShortFrameSpeed();
    rightLayout->addWidget(speedTagLabel);
    rightLayout->addWidget(m_GifSpeedComboBox);


    // 屏幕录制生成gif的文件名
    QLabel *screenGifTagLabel = new QLabel("屏幕录制生成gif文件的文件名：");
    QString pathString = g_UIGlobalConfig->getScreenShortGifPath();
    m_ScreenGifLineEdit = new QLineEdit(pathString);
    rightLayout->addWidget(screenGifTagLabel);
    rightLayout->addWidget(m_ScreenGifLineEdit);

    // 文件生成的gif的文件名
    QLabel *fileGifTagLabel = new QLabel("多文件生成gif文件的文件名：");
    pathString = g_UIGlobalConfig->getOtherFileGifPath();
    m_FileGifLineEdit = new QLineEdit(pathString);
    rightLayout->addWidget(fileGifTagLabel);
    rightLayout->addWidget(m_FileGifLineEdit);

    m_MainLayout->addLayout(layout);
    m_WorkMode = None_Mode;

    initSystemTrayIcon();
    m_ProgressWindow = new UIProgressWindow;
    m_ProgressWindow->setCustomerWindowTitle("图片合成进度");
    m_ProgressWindow->resize(300, 130);
    m_ProgressWindow->hide();

    // 连接信号和槽
    QObject::connect(m_ScreenShot, SIGNAL(clicked()), this, SLOT(onScreenShortButtonClicked()));
    QObject::connect(m_GifStartButton, SIGNAL(clicked()), this, SLOT(onGifStartClicked()));
    QObject::connect(m_GifEndButton, SIGNAL(clicked()), this, SLOT(onGifEndClicked()));
    QObject::connect(m_ImportGifButton, SIGNAL(clicked()), this, SLOT(onImportGifFileClicked()));
    QObject::connect(m_GifResolveButton, SIGNAL(clicked()), this, SLOT(onGifResolveClicked()));
    QObject::connect(m_ImportOtherPicButton, SIGNAL(clicked()), this, SLOT(onImportOtherFilesClicked()));
    QObject::connect(m_CreateGifButton, SIGNAL(clicked()), this, SLOT(onGifCreatClicked()));
    QObject::connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    QObject::connect(m_ScreenShortWidget, SIGNAL(signalFinishedScreenShort(QRect)), \
                     this, SLOT(onScreenShortFinished(QRect)));

    QObject::connect(m_ScreenShortHideThis, SIGNAL(currentIndexChanged(int)), \
                     this, SLOT(onChangeShortCutMainWidgetShowOrHide(int)));
    QObject::connect(m_GifSpeedComboBox, SIGNAL(currentIndexChanged(int)), \
                     this, SLOT(onChangeGifCreateSpeed(int)));
    QObject::connect(g_GifCreater, SIGNAL(signalCurrentSubPage(int)), this, \
                     SLOT(onChangeGifProgress(int)));
    QObject::connect(m_ScreenGifLineEdit, &QLineEdit::textChanged, [=](QString str)->void{
        g_UIGlobalConfig->setScreenShortGifPath(str);
    });
    QObject::connect(m_FileGifLineEdit, &QLineEdit::textChanged, [=](QString str)->void{
        g_UIGlobalConfig->setOtherFileGifPath(str);
    });

    m_GifSpeedComboBox->setCurrentIndex(index);
    m_ScreenShortHideThis->setCurrentIndex(isHide ? 0 : 1);
}

GifBuilderWidget::~GifBuilderWidget()
{

}

void GifBuilderWidget::onScreenShortButtonClicked(void)
{
    m_WorkMode = ScreenShort_Mode;
    m_ScreenShortWidget->show();

    // 隐藏主窗口显示
    if (!m_ScreenShortIsShowMainWidget && !this->isHidden())
        this->hide();
}

void GifBuilderWidget::onGifStartClicked(void)
{
    m_WorkMode = GifCreater_Mode;
    m_ScreenShortWidget->show();

    // 隐藏主窗口显示
    if (!m_ScreenShortIsShowMainWidget && !this->isHidden())
        this->hide();
}

void GifBuilderWidget::onGifEndClicked(void)
{
    m_WorkMode = None_Mode;
    m_GifEndButton->setEnabled(false);
    m_GifStartButton->setEnabled(true);

    m_Timer->stop();
    g_GifCreater->endGifCreater();
    g_GifCreater->requestInterruption();
    g_GifCreater->quit();

    QString fileName = "./ShortCutGif/" + m_ScreenGifLineEdit->text();
    m_ImageView->setGifPic(fileName);
}

void GifBuilderWidget::onImportGifFileClicked(void)
{
    QString nCurrentString = QFileDialog::getOpenFileName(this, "打开gif文件", "./", tr("Image Files (*.gif)"));
    if (!nCurrentString.isEmpty())
    {
        m_GifResolveButton->setEnabled(true);
        m_CurrentGifFileName = nCurrentString;
        m_ImageView->setGifPic(nCurrentString);
    }
}

void GifBuilderWidget::onGifResolveClicked(void)
{
    QString fileName = m_CurrentGifFileName;
    if (fileName.isEmpty())
        return;

    resolveGifFile(fileName);
    UIMessageBox message;
    message.abort("图片分解状态", "图片分解完成");

    m_GifResolveButton->setEnabled(false);
}

void GifBuilderWidget::onImportOtherFilesClicked(void)
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "打开gif文件", "./", \
                                                          tr("Image Files (*.png *jpg *bmp)"));
    if (fileNames.count() <= 0)
        return;

    QList<QPixmap> pixmapList;
    pixmapList.clear();
    for (int i=0; i<fileNames.count(); ++i)
    {
        QString fileName = fileNames.at(i);
        QPixmap pixmap;
        bool result = pixmap.load(fileName);
        if (!result)
            continue;
        pixmapList.push_back(pixmap);
    }
    m_ImageView->setPixmapList(pixmapList);
    m_CreateGifButton->setEnabled(true);
}

void GifBuilderWidget::onGifCreatClicked(void)
{
    QList<QPixmap> nPixmapList = m_ImageView->getCurrentPixmapList();
    if (nPixmapList.count() <= 0)
        return;

    QString fileName = "./OtherFilesGif/" + m_FileGifLineEdit->text();
    m_TotalCount = nPixmapList.count();
    m_ProgressWindow->show();
    for (int i=0; i<nPixmapList.count(); ++i)
    {
        QPixmap pixmap = nPixmapList.at(i);
        bool result = !(pixmap.isNull());
        if (result)
        {
            g_GifCreater->setSpeed(m_GifCreateSpeed);
            g_GifCreater->setGifPicSize(pixmap.width(), pixmap.height());
            g_GifCreater->addPicList(pixmap);
            if (!g_GifCreater->isRunning())
            {
                g_GifCreater->startGifCreater(fileName);
                g_GifCreater->start();
            }
        }
    }
}

void GifBuilderWidget::onScreenShortFinished(QRect rect)
{
    if (m_WorkMode == ScreenShort_Mode)
    {
        QDateTime time = QDateTime::currentDateTime();
        QString imgPathString = "./ShortCutPictures/%1-%2-%3 %4-%5-%6.png";
        imgPathString = imgPathString.arg(time.date().year()).arg(time.date().month()).arg(time.date().day()) \
                .arg(time.time().hour()).arg(time.time().minute()).arg(time.time().second());
        saveToFile(rect, imgPathString);

        if (this->isHidden())
            this->show();
    }
    else if (m_WorkMode == GifCreater_Mode)
    {
        m_GifEndButton->setEnabled(true);
        m_GifStartButton->setEnabled(false);

        QPixmap pixmap = getCurrentPixmap(rect);
        m_TotalCount = 0;
        g_GifCreater->setSpeed(m_GifCreateSpeed);
        g_GifCreater->setGifPicSize(rect.width(), rect.height());
        g_GifCreater->addPicList(pixmap);

        // 开启线程并合成图片
        m_TimerStart = true;
        QString fileName = "./ShortCutGif/" + m_ScreenGifLineEdit->text();
        g_GifCreater->startGifCreater(fileName);
        if (!g_GifCreater->isRunning())
            g_GifCreater->start();

        m_Timer->start();
    }
}

void GifBuilderWidget::onTimeout(void)
{
    if (m_WorkMode == GifCreater_Mode)
    {
        if (m_ScreenShortWidget == nullptr)
            return;
        QRect rect = m_ScreenShortWidget->getRect();
        QPixmap pixmap = getCurrentPixmap(rect);
        g_GifCreater->addPicList(pixmap);
    }
}

void GifBuilderWidget::onChangeShortCutMainWidgetShowOrHide(int index)
{
    if (index == 0)
        m_ScreenShortIsShowMainWidget = true;
    else
        m_ScreenShortIsShowMainWidget = false;

    g_UIGlobalConfig->setScreenShortMainWidgetShow(m_ScreenShortIsShowMainWidget);
}

void GifBuilderWidget::onChangeGifCreateSpeed(int index)
{
    if (index == 0)
        m_GifCreateSpeed = 100;
    else if (index == 1)
        m_GifCreateSpeed = 50;
    else if(index == 2)
        m_GifCreateSpeed = 20;
    else
        m_GifCreateSpeed = 10;

    m_Timer->setInterval(1000 / (100.0 / m_GifCreateSpeed));
    g_UIGlobalConfig->setScreenShortFrameSpeed(index);
}

void GifBuilderWidget::onChangeGifProgress(int value)
{
    if (m_TotalCount <= 0)
    {
        m_ProgressWindow->hide();
        return;
    }

    int number = m_TotalCount - value;
    QString str = "正在合成第%1张";
    str = str.arg(number);
    m_ProgressWindow->setProgressValue(number * 1.0 / m_TotalCount * 100);
    m_ProgressWindow->setTagContent(str);

    if (value == 0)
    {
        g_GifCreater->endGifCreater2();
        m_ProgressWindow->hide();
        g_GifCreater->requestInterruption();
        g_GifCreater->quit();

        QString fileName = "./OtherFilesGif/" + m_FileGifLineEdit->text();
        UIMessageBox messageBox;
        messageBox.abort("提示", "图片合成完成！");
        m_ImageView->setGifPic(fileName);
        m_CreateGifButton->setEnabled(false);
    }
}

void GifBuilderWidget::saveToFile(QRect rect, QString filePathString)
{
    QPixmap pixmap = getCurrentPixmap(rect);
    m_ImageView->setPixmap(pixmap);
    pixmap.save(filePathString, "png");
}

QPixmap GifBuilderWidget::getCurrentPixmap(QRect rect)
{
    QScreen *screen = QApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
    return pixmap;
}

void GifBuilderWidget::resolveGifFile(QString filePath)
{
    QMovie movie(filePath);
    if (!movie.isValid())
        return;

    QFile file(filePath);
    QString fileName = file.fileName();
    fileName = fileName.left(fileName.length() - 4);

    int nFrameCount = movie.frameCount();
    for (int i=0; i<nFrameCount; ++i)
    {
        if (movie.jumpToFrame(i))
        {
            QPixmap pixmap = movie.currentPixmap();
            pixmap.save(fileName + QString::number(i + 1) + ".png", "png");
        }
    }
}

// 初始化托盘程序
void GifBuilderWidget::initSystemTrayIcon(void)
{
    // 初始化托盘相关
    m_SystemTrayIcon = new QSystemTrayIcon(this);
    m_SystemTrayIcon->setIcon(QIcon("./Icons/AppIconD.png"));
    m_SystemTrayIcon->setToolTip("Easy Gif Tool v1.0");
    m_SystemTrayIcon->show();

    QObject::connect(m_SystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), \
                     this, SLOT(onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason)));

    // 初始化托盘的菜单
    m_SystemTrayIconMenu = new QMenu(this);
    m_SystemTrayIcon->setContextMenu(m_SystemTrayIconMenu);
    QAction *nScreenShort = new QAction("屏幕截图", m_SystemTrayIconMenu);
    QAction *nGifStart = new QAction("开始录制Gif", m_SystemTrayIconMenu);
    QAction *nGifEnd = new QAction("结束录制Gif", m_SystemTrayIconMenu);
    QAction *nShow = new QAction("显示主界面", m_SystemTrayIconMenu);
    QAction *nExit = new QAction("退出应用程序", m_SystemTrayIconMenu);

    m_SystemTrayIconMenu->addAction(nScreenShort);
    m_SystemTrayIconMenu->addAction(nGifStart);
    m_SystemTrayIconMenu->addAction(nGifEnd);
    m_SystemTrayIconMenu->addSeparator();
    m_SystemTrayIconMenu->addAction(nShow);
    m_SystemTrayIconMenu->addAction(nExit);

    // 建立信号和槽的连接
    QObject::connect(nScreenShort, SIGNAL(triggered()), this, SLOT(onScreenShortButtonClicked()));
    QObject::connect(nGifStart, SIGNAL(triggered()), this, SLOT(onGifStartClicked()));
    QObject::connect(nGifEnd, SIGNAL(triggered()), this, SLOT(onGifEndClicked()));
    QObject::connect(nShow, &QAction::triggered, [=](void)->void{
        if (this->isMinimized())
            this->showNormal();
        this->show();
    });
    QObject::connect(nExit, &QAction::triggered, [=](void)->void{
        this->close();
    });
}

// 托盘的事件相应
void GifBuilderWidget::onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
        this->show();
}
