#pragma once
#include "user_action.h"

namespace hal
{
    /**
     * @ingroup user_action
     */
    class ActionPythonOpenFile : public UserAction
    {
        u32 mTabIndex;
        QString mFilename;
    public:
        ActionPythonOpenFile(const u32& tabIndex_ = 0, const QString& filename_ = QString());
        QString tagname() const override;
        bool exec() override;
        void writeToXml(QXmlStreamWriter& xmlOut) const override;
        void readFromXml(QXmlStreamReader& xmlIn) override;
        void addToHash(QCryptographicHash& cryptoHash) const override;
    };

    /**
     * @ingroup user_action
     */
    class ActionPythonOpenFileFactory : public UserActionFactory
    {
    public:
        ActionPythonOpenFileFactory();
        UserAction* newAction() const;
        static ActionPythonOpenFileFactory* sFactory;
    };
}
