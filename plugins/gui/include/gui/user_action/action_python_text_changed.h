#pragma once
#include "user_action.h"

namespace hal
{
    /**
     * @ingroup user_action
     */
    class ActionPythonTextChanged : public UserAction
    {
        QString mText;
    public:
        ActionPythonTextChanged(const QString& text_ = QString());
        QString tagname() const override;
        bool exec() override;
        void writeToXml(QXmlStreamWriter& xmlOut) const override;
        void readFromXml(QXmlStreamReader& xmlIn) override;
        void addToHash(QCryptographicHash& cryptoHash) const override;
    };

    /**
     * @ingroup user_action
     */
    class ActionPythonTextChangedFactory : public UserActionFactory
    {
    public:
        ActionPythonTextChangedFactory();
        UserAction* newAction() const;
        static ActionPythonTextChangedFactory* sFactory;
    };
}
