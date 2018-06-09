#ifndef GIF_BUILDER_WIDGET_H
#define GIF_BUILDER_WIDGET_H
#include "UIBase/UIBaseWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QComboBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ScreenShortWidget.h"
#include "ImageView.h"
#include "UIBase/UIProgressWindow.h"
#include "UIBase/UIGlobalConfig.h"

class GifBuilderWidget : public UIBaseWindow
{
    Q_OBJECT
public:
    enum WorkMode
    {
        None_Mode,
        ScreenShort_Mode,
        GifCreater_Mode
    };

public:
    GifBuilderWidget(QWidget *parent = nullptr);
    ~GifBuilderWidget();

private:
    QVBoxLayout *m_MainLayout = nullptr;

    QWidget *m_LeftWidget = nullptr;
    QWidget *m_RightWidget = nullptr;

    // 截图按钮
    QPushButton *m_ScreenShot = nullptr;
    // 开始录制Gif按钮
    QPushButton *m_GifStartButton = nullptr;
    // 结束录制Gif按钮
    QPushButton *m_GifEndButton= nullptr;
    // 导入Gif文件
    QPushButton *m_ImportGifButton = nullptr;
    // 分解Gif文件
    QPushButton *m_GifResolveButton = nullptr;
    // 导入多文件
    QPushButton *m_ImportOtherPicButton = nullptr;
    // 合成多个Gif图片
    QPushButton *m_CreateGifButton = nullptr;

    // 选择Gif合成的速率
    QComboBox *m_GifSpeedComboBox = nullptr;
    // 屏幕录制生成gif的文件名
    QLineEdit *m_ScreenGifLineEdit = nullptr;
    // 文件生成的gif的文件名
    QLineEdit *m_FileGifLineEdit = nullptr;
    // 截图时是否隐藏本软件
    QComboBox *m_ScreenShortHideThis= nullptr;

    // 截取图片窗口
    ScreenShortWidget *m_ScreenShortWidget = nullptr;
    ImageView *m_ImageView = nullptr;
    WorkMode m_WorkMode;

private slots:
    void onScreenShortButtonClicked(void);
    void onGifStartClicked(void);
    void onGifEndClicked(void);
    void onImportGifFileClicked(void);
    void onGifResolveClicked(void);
    void onImportOtherFilesClicked(void);
    void onGifCreatClicked(void);
    void onScreenShortFinished(QRect);
    void onTimeout(void);

    // 托盘的事件相应
    void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason);
    // 改变截图时主界面的显隐状态
    void onChangeShortCutMainWidgetShowOrHide(int);
    // 改变截图的录制速度
    void onChangeGifCreateSpeed(int);
    // 图片合成进度
    void onChangeGifProgress(int);


private:
    void saveToFile(QRect rect, QString filePathString);
    QPixmap getCurrentPixmap(QRect rect);
    // 分解一个Gif图片成多个图片
    void resolveGifFile(QString filePath);

    QTimer *m_Timer = nullptr;
    bool m_TimerStart = false;

    // 托盘
    QSystemTrayIcon *m_SystemTrayIcon = nullptr;
    // 初始化托盘
    void initSystemTrayIcon(void);
    QMenu *m_SystemTrayIconMenu = nullptr;

    // 配置相关
    bool m_ScreenShortIsShowMainWidget = true;      // 截图时是否显示主窗口
    int m_GifCreateSpeed = 50;                      // Gif的录制速度

    // 当前导入的Gif文件名称
    QString m_CurrentGifFileName;

    // 进度窗口
    UIProgressWindow *m_ProgressWindow = nullptr;
    // 总图片数目
    int m_TotalCount = 0;
};

#endif
