#include "UIGlobalConfig.h"
#include <QFile>
#include <QSettings>

UIGlobalConfig::UIGlobalConfig(QObject *parent)
    :QObject(parent)
{
    m_showScreenShortHide = false;
    m_FrameSpeed = 10;
    m_ScreenShortGifPath = "Test.gif";
    m_OtherFileGifPath = "CreateGif.gif";

    loadConfigFile();
}

UIGlobalConfig::~UIGlobalConfig()
{

}

UIGlobalConfig *UIGlobalConfig::getInstance(void)
{
    static UIGlobalConfig instance;
    return &instance;
}

void UIGlobalConfig::loadConfigFile(void)
{
    QString configFile = "./config/GlobalConfig.ini";

    QSettings settings(configFile, QSettings::IniFormat);
    m_showScreenShortHide = settings.value("GlobalConfig/ScreenShortHide").toBool();
    m_FrameSpeed = settings.value("GlobalConfig/FreamSpeed").toInt();
    m_ScreenShortGifPath = settings.value("GlobalConfig/ScreenShortGifCreate").toString();
    m_OtherFileGifPath = settings.value("GlobalConfig/OtherFileGifCreate").toString();
}

void UIGlobalConfig::syncConfigFile(void)
{
    QString configFile = "./config/GlobalConfig.ini";

    QSettings settings(configFile, QSettings::IniFormat);
    settings.setValue("GlobalConfig/ScreenShortHide", m_showScreenShortHide);
    settings.setValue("GlobalConfig/FreamSpeed", m_FrameSpeed);
    settings.setValue("GlobalConfig/ScreenShortGifCreate", m_ScreenShortGifPath);
    settings.setValue("GlobalConfig/OtherFileGifCreate", m_OtherFileGifPath);
}

bool UIGlobalConfig::getScreenShortMainWidgetShow(void)
{
    return m_showScreenShortHide;
}

void UIGlobalConfig::setScreenShortMainWidgetShow(bool isShow)
{
    m_showScreenShortHide = isShow;
    syncConfigFile();
}

int UIGlobalConfig::getScreenShortFrameSpeed(void)
{
    return m_FrameSpeed;
}

void UIGlobalConfig::setScreenShortFrameSpeed(int speed)
{
    m_FrameSpeed = speed;
    syncConfigFile();
}

QString UIGlobalConfig::getScreenShortGifPath(void)
{
    return m_ScreenShortGifPath;
}

void UIGlobalConfig::setScreenShortGifPath(QString path)
{
    m_ScreenShortGifPath = path;
    syncConfigFile();
}

QString UIGlobalConfig::getOtherFileGifPath(void)
{
    return m_OtherFileGifPath;
}

void UIGlobalConfig::setOtherFileGifPath(QString path)
{
    m_OtherFileGifPath = path;
    syncConfigFile();
}
