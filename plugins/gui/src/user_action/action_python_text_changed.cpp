#include "gui/user_action/action_python_text_changed.h"
#include <QDebug>
#include <QTabWidget>
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonTextChangedFactory::ActionPythonTextChangedFactory()
       : UserActionFactory("PythonTextChanged") {;}

    ActionPythonTextChangedFactory* ActionPythonTextChangedFactory::sFactory = new ActionPythonTextChangedFactory;

    UserAction* ActionPythonTextChangedFactory::newAction() const
    {
        return new ActionPythonTextChanged;
    }

    QString ActionPythonTextChanged::tagname() const
    {
        return ActionPythonTextChangedFactory::sFactory->tagname();
    }

    // maximum duration for single text change 10 sec
    qint64 ActionPythonTextChanged::sRecentTextChangeMsec = 10000;

    ActionPythonTextChanged::ActionPythonTextChanged(const u32 &id_, const QString &oldtext_, const QString &text_)
        : mOldText(oldtext_), mText(text_), mPythonCodeEditorId(id_), mLastKeyIsReturn(false), mMerged(false), mDuration(0)
    {
        if (id_)
            setObject(UserActionObject(id_,UserActionObjectType::PythonCodeEditor));
    }

    bool ActionPythonTextChanged::exec()
    {
        if(UserActionManager::instance()->isUserTriggeredAction()) {
            PythonCodeEditor* pythonCodeEditor = gContentManager->getPythonEditorWidget()->getPythonCodeEditorById(mPythonCodeEditorId);
            pythonCodeEditor->setOldPlainText(mText);
        }

        if (mergeWithRecent())
        {
            // caller should delete this action to avoid memory leak
            mMerged = true;
            return true;
        }
        if(!mUndoAction) {
            mUndoAction = new ActionPythonTextChanged(mPythonCodeEditorId, "", mText);
        }

        if(!UserActionManager::instance()->isUserTriggeredAction()) {
            PythonCodeEditor* pythonCodeEditor = gContentManager->getPythonEditorWidget()->getPythonCodeEditorById(mPythonCodeEditorId);
            if(!pythonCodeEditor) {
                gContentManager->getPythonEditorWidget()->newTab(mPythonCodeEditorId);
                pythonCodeEditor = gContentManager->getPythonEditorWidget()->getPythonCodeEditorById(mPythonCodeEditorId);
                if(!pythonCodeEditor) return false;
            }
            pythonCodeEditor->setPlainText(mText);
        }
        return UserAction::exec();
    }

    bool ActionPythonTextChanged::mergeWithRecent()
    {
        if(mLastKeyIsReturn) return false;

        UserActionManager* uam = UserActionManager::instance();
        Q_ASSERT(uam);
        if (uam->mActionHistory.isEmpty()) return false;
        UserAction* lastAction = uam->mActionHistory.last();
        qint64 msecSinceLastAction = uam->timeStamp() - lastAction->timeStamp();
        if (msecSinceLastAction > sRecentTextChangeMsec) return false;
        ActionPythonTextChanged* lastTextChanged = dynamic_cast<ActionPythonTextChanged*>(lastAction);
        if (!lastTextChanged) return false;
        if (lastTextChanged->mPythonCodeEditorId != mPythonCodeEditorId) return false;
        lastTextChanged->mTimeStamp = uam->timeStamp();
        lastTextChanged->mText      = mText;
        lastTextChanged->mDuration += msecSinceLastAction;
        return true;
    }

    void ActionPythonTextChanged::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(mText.toUtf8());
        cryptoHash.addData(QByteArray::number(mPythonCodeEditorId));
    }

    void ActionPythonTextChanged::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("pythonscript", mText);
        xmlOut.writeTextElement("uid", QString::number(mPythonCodeEditorId));
        if (mDuration > 0)
            xmlOut.writeTextElement("duration", QString::number(mDuration));
    }

    void ActionPythonTextChanged::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "pythonscript")
                mText = xmlIn.readElementText();
            if (xmlIn.name() == "uid")
                mPythonCodeEditorId = xmlIn.readElementText().toInt();
        }
    }

    void ActionPythonTextChanged::setLastKeyIsReturn()
    {
        mLastKeyIsReturn = true;
    }
}
