#include "selection_details_widget/disputed_big_icon.h"
#include "gui_globals.h"

namespace hal
{
    DisputedBigIcon::DisputedBigIcon(const QString &iconName, QWidget *parent)
        : QLabel(parent)
    {
        if (!iconName.isEmpty())
            setPixmap(QPixmap(QString(":/icons/%1").arg(iconName),"PNG").scaled(64,64));
        setFixedSize(68,68);
        connect(&g_settings_relay, &SettingsRelay::setting_changed, this, &DisputedBigIcon::handleGlobalSettingsChanged);
        mIsVisible = g_settings_manager.get("selection_details/show_big_icon", true).toBool();
        showOrHide();
    }

    void DisputedBigIcon::showOrHide()
    {
        if (mIsVisible)
            show();
        else
            hide();
    }

    void DisputedBigIcon::handleGlobalSettingsChanged(void* sender, const QString& key, const QVariant& value)
    {
        Q_UNUSED(sender)
        if (key == "selection_details/show_big_icon")
        {
            bool val = value.toBool();
            if (val != mIsVisible)
            {
                mIsVisible = val;
                showOrHide();
            }
        }

    }
}
