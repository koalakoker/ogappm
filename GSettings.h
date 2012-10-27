#ifndef GSETTINGS_H
#define GSETTINGS_H

#include <QList>
#include "GSettingsItem.h"
#include "GSettingDefines.h"

class GSettings : public QList<GSettingsItem*>
{
public:
    GSettings(const QString & organization, const QString & application);
    void SaveConfig(void);
    void LoadConfig(void);
    void RestoreDefault(void);
    GSettingsItem* Get(QString confStr);
    void SetValue(QString confStr,QVariant value);
private:
    QString m_organization;
    QString m_application;
};

#endif // GSETTINGS_H
