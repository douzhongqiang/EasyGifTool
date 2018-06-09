#ifndef UIGLOBAL_CONFIG_H
#define UIGLOBAL_CONFIG_H

#include <QObject>
#include <QDomDocument>

#define g_UIGlobalConfig UIGlobalConfig::getInstance()

class UIGlobalConfig : public QObject
{
    Q_OBJECT

private:
    UIGlobalConfig(QObject *parent = nullptr);
    ~UIGlobalConfig();

public:
    static UIGlobalConfig *getInstance(void);

    // 加载XML文件
    void loadConfigFile(void);
    // 同步配置到配置文件
    void syncConfigFile(void);
    // 获取截图时是够显示主窗口
    bool getScreenShortMainWidgetShow(void);
    // 设置截图时是够显示主窗口
    void setScreenShortMainWidgetShow(bool);
    // 获取动画帧率速度
    int getScreenShortFrameSpeed(void);
    // 设置动画帧率速度
    void setScreenShortFrameSpeed(int);
    // 获取截屏gif文件路径
    QString getScreenShortGifPath(void);
    // 设置截屏gif文件路径
    void setScreenShortGifPath(QString);
    // 获取多文件合成gif文件路径
    QString getOtherFileGifPath(void);
    // 设置多文件合成gif文件路径
    void setOtherFileGifPath(QString);

private:
    bool m_showScreenShortHide;
    int m_FrameSpeed;
    QString m_ScreenShortGifPath;
    QString m_OtherFileGifPath;
};

#endif
