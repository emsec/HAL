#pragma once
#include "user_action.h"

namespace hal
{
    /**
     * @ingroup user_action
     */
    class ActionPythonTabChanged : public UserAction
    {
        u32 mNewIndex;
    public:
        ActionPythonTabChanged(const u32& newIndex_ = -1);
        QString tagname() const override;
        bool exec() override;
        void writeToXml(QXmlStreamWriter& xmlOut) const override;
        void readFromXml(QXmlStreamReader& xmlIn) override;
        void addToHash(QCryptographicHash& cryptoHash) const override;
    };

    /**
     * @ingroup user_action
     */
    class ActionPythonTabChangedFactory : public UserActionFactory
    {
    public:
        ActionPythonTabChangedFactory();
        UserAction* newAction() const;
        static ActionPythonTabChangedFactory* sFactory;
    };
}
