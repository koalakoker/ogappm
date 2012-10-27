#ifndef GSETTINGSITEM_H
#define GSETTINGSITEM_H

#include <QVariant>

class GSettingsItem
{
public:
    GSettingsItem(QString confStr, QVariant defValue);
    void SetValue(QVariant value);
    QVariant Value(void);
    QString ConfStr(void);
    QVariant DefValue(void);
    void RestoreDefault(void);

private:
    QVariant m_value;
    QVariant m_defValue;
    QString m_confStr;
};

#endif // GSETTINGSITEM_H
