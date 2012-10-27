#include "GSettings.h"
#include <QSettings>

GSettings::GSettings(const QString & organization, const QString & application)
{
    m_organization = organization;
    m_application = application;
}

void GSettings::SaveConfig(void)
{
    QSettings settings(m_organization, m_application);

    settings.beginGroup("Settings");
    int i;
    for (i = 0; i < count(); i++)
    {
        GSettingsItem* settingItem = at(i);
        settings.setValue(settingItem->ConfStr(), settingItem->Value());
    }
    settings.endGroup();
}

void GSettings::LoadConfig(void)
{
    QSettings settings(m_organization, m_application);

    settings.beginGroup("Settings");
    int i;
    for (i = 0; i < count(); i++)
    {
        GSettingsItem* settingItem = at(i);
        settingItem->SetValue(settings.value(settingItem->ConfStr(),settingItem->DefValue()));
    }
    settings.endGroup();
}

void GSettings::RestoreDefault(void)
{
    int i;
    for (i = 0; i < count(); i++)
    {
        GSettingsItem* settingItem = at(i);
        settingItem->RestoreDefault();
    }
}

GSettingsItem* GSettings::Get(QString confStr)
{
    GSettingsItem* retVal = NULL;
    int i;
    for (i = 0; i < count(); i++)
    {
        GSettingsItem* settingItem = at(i);
        if (settingItem->ConfStr()==confStr)
        {
            retVal = settingItem;
            break;
        }
    }
    return retVal;
}

void GSettings::SetValue(QString confStr,QVariant value)
{
    int i;
    for (i = 0; i < count(); i++)
    {
        GSettingsItem* settingItem = at(i);
        if (settingItem->ConfStr()==confStr)
        {
            settingItem->SetValue(value);
        }
    }
}
