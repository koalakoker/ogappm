#include "GSettingsItem.h"

GSettingsItem::GSettingsItem(QString confStr, QVariant defValue)
{
    m_confStr = confStr;
    m_value = defValue;
    m_defValue = defValue;
}

void GSettingsItem::SetValue(QVariant value)
{
    m_value = value;
}

QVariant GSettingsItem::Value(void)
{
    return m_value;
}

void GSettingsItem::RestoreDefault(void)
{
    m_value = m_defValue;
}

QString GSettingsItem::ConfStr(void)
{
    return m_confStr;
}

QVariant GSettingsItem::DefValue(void)
{
    return m_defValue;
}
