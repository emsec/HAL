#pragma once
#include "user_action.h"

namespace hal
{
    /**
     * @ingroup user_action
     */
    class ActionPythonSaveFile : public UserAction
    {
        u32 mPythonCodeEditorId;
        QString mFilename;
    public:
        ActionPythonSaveFile(const u32& id_ = 0, const QString& filename_ = QString());
        QString tagname() const override;
        bool exec() override;
        void writeToXml(QXmlStreamWriter& xmlOut) const override;
        void readFromXml(QXmlStreamReader& xmlIn) override;
        void addToHash(QCryptographicHash& cryptoHash) const override;
    };

    /**
     * @ingroup user_action
     */
    class ActionPythonSaveFileFactory : public UserActionFactory
    {
    public:
        ActionPythonSaveFileFactory();
        UserAction* newAction() const;
        static ActionPythonSaveFileFactory* sFactory;
    };
}
