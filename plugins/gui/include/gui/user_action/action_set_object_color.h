#pragma once
#include "user_action.h"
#include <QColor>

namespace hal
{
    class ActionSetObjectColor : public UserAction
    {
        QColor mColor;
    public:
        ActionSetObjectColor(const QColor& col=QColor()) : mColor(col) {;}
        void exec() override;
        QString tagname() const override;
        void writeToXml(QXmlStreamWriter& xmlOut) const override;
        void readFromXml(QXmlStreamReader& xmlIn) override;
        void addToHash(QCryptographicHash& cryptoHash) const override;
    };

    class ActionSetObjectColorFactory : public UserActionFactory
    {
    public:
        ActionSetObjectColorFactory();
        UserAction* newAction() const;
        static ActionSetObjectColorFactory* sFactory;
    };
}
