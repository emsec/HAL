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

    ActionPythonTextChanged::ActionPythonTextChanged(const QString &oldtext_, const QString &text_)
        : mOldText(oldtext_), mText(text_), mMerged(false), mDuration(0)
    {;}

    bool ActionPythonTextChanged::exec()
    {
        if (mergeWithRecent())
        {
            // caller should delete this action to avoid memory leak
            mMerged = true;
            return true;
        }
        if(!mUndoAction) {
            mUndoAction = new ActionPythonTextChanged("", mText);
            dynamic_cast<ActionPythonTextChanged*>(mUndoAction)->setPythonCodeEditorUUID(mPythonCodeEditorUUID);
        }

        if(UserActionManager::instance()->isUserTriggeredAction()) {
            // TODO remove
            /* qDebug() << QString("UserAction triggered is an user triggered action");
            qDebug() << mText;
            qDebug() << mOldText;
            qDebug() << mPythonCodeEditorUUID; */
            PythonCodeEditor* pythonCodeEditor = PythonCodeEditor::getPythonCodeEditorByUUID(mPythonCodeEditorUUID);
            pythonCodeEditor->setOldPlainText(mText);
        } else {
            // TODO remove
            /* qDebug() << QString("UserAction triggered is no user triggered action");
            qDebug() << mText;
            qDebug() << mOldText;
            qDebug() << mPythonCodeEditorUUID; */
            PythonCodeEditor* pythonCodeEditor = PythonCodeEditor::getPythonCodeEditorByUUID(mPythonCodeEditorUUID);
            pythonCodeEditor->setPlainText(mText);
        }
        return UserAction::exec();
    }

    bool ActionPythonTextChanged::mergeWithRecent()
    {
        UserActionManager* uam = UserActionManager::instance();
        Q_ASSERT(uam);
        if (uam->mActionHistory.isEmpty()) return false;
        UserAction* lastAction = uam->mActionHistory.last();
        qint64 msecSinceLastAction = uam->timeStamp() - lastAction->timeStamp();
        if (msecSinceLastAction > sRecentTextChangeMsec) return false;
        ActionPythonTextChanged* lastTextChanged = dynamic_cast<ActionPythonTextChanged*>(lastAction);
        if (!lastTextChanged) return false;
        if (lastTextChanged->mPythonCodeEditorUUID != mPythonCodeEditorUUID) return false;
        lastTextChanged->mTimeStamp = uam->timeStamp();
        lastTextChanged->mText      = mText;
        lastTextChanged->mDuration += msecSinceLastAction;
        return true;
    }

    void ActionPythonTextChanged::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(mText.toUtf8());
    }

    void ActionPythonTextChanged::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("pythonscript", mText);
        if (mDuration > 0)
            xmlOut.writeTextElement("duration", QString::number(mDuration));
    }

    void ActionPythonTextChanged::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "pythonscript")
                mText = xmlIn.readElementText();
        }
    }

    void ActionPythonTextChanged::setPythonCodeEditorUUID(QUuid &uuid_)
    {
         mPythonCodeEditorUUID = uuid_;
    }
}
