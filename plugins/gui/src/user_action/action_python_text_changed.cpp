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
        : mOldText(oldtext_), mText(text_), mLastKeyIsReturn(false), mMerged(false), mDuration(0)
    {;}

    bool ActionPythonTextChanged::exec()
    {
        if(UserActionManager::instance()->isUserTriggeredAction()) {
            PythonCodeEditor* pythonCodeEditor = PythonCodeEditor::getPythonCodeEditorByUUID(mPythonCodeEditorUUID);
            pythonCodeEditor->setOldPlainText(mText);
        }

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

        if(!UserActionManager::instance()->isUserTriggeredAction()) {
            PythonCodeEditor* pythonCodeEditor = PythonCodeEditor::getPythonCodeEditorByUUID(mPythonCodeEditorUUID);
            // workaround, if we are playing a macro the uuid of the python code editor is unknown
            // TODO remove we are always using the current widget
            if(!pythonCodeEditor) {
                QTabWidget* qTabWidget = gContentManager->getPythonEditorWidget()->getTabWidget();
                pythonCodeEditor = dynamic_cast<PythonCodeEditor*>(qTabWidget->currentWidget());
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

    void ActionPythonTextChanged::setLastKeyIsReturn()
    {
        mLastKeyIsReturn = true;
    }
}
