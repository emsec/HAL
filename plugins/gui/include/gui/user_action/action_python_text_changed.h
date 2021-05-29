#pragma once
#include "user_action.h"
#include "gui/python/python_code_editor.h"

namespace hal
{
    /**
     * @ingroup user_action
     */
    class ActionPythonTextChanged : public UserAction
    {
        QString mOldText;
        QString mText;
        u32 mPythonCodeEditorId;

        bool mLastKeyIsReturn;
        bool mMerged;

        qint64 mDuration;

        // merge with previous action if recent text change in event queue
        bool mergeWithRecent();

        // maximum duration in msec for single text change
        static qint64 sRecentTextChangeMsec;
    public:
        ActionPythonTextChanged(const u32& id_ = 0, const QString& oldtext_ = QString(), const QString& text_ = QString());
        QString tagname() const override;
        bool exec() override;
        void writeToXml(QXmlStreamWriter& xmlOut) const override;
        void readFromXml(QXmlStreamReader& xmlIn) override;
        void addToHash(QCryptographicHash& cryptoHash) const override;

        // needs to be deleted by caller since action was merged and not executed
        bool wasMergedWithRecent() const {return mMerged; }

        void setLastKeyIsReturn();
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
