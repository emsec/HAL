#include "gui/user_action/action_python_text_changed.h"

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

    ActionPythonTextChanged::ActionPythonTextChanged(const QString &text_)
        : mText(text_), mMerged(false), mDuration(0)
    {;}

    bool ActionPythonTextChanged::exec()
    {
        if (mergeWithRecent())
        {
            // caller should delete this action to avoid memory leak
            mMerged = true;
            return true;
        }
        return UserAction::exec();
    }

    bool ActionPythonTextChanged::mergeWithRecent()
    {
        UserActionManager* uam = UserActionManager::instance();
        Q_ASSERT(uam);
        if (uam->mActionHistory.isEmpty()) return false;
        UserAction* lastAction = uam->mActionHistory.last();
        if (uam->timeStamp() - lastAction->timeStamp() > sRecentTextChangeMsec) return false;
        ActionPythonTextChanged* lastTextChanged = dynamic_cast<ActionPythonTextChanged*>(lastAction);
        if (!lastTextChanged) return false;
        lastTextChanged->mText = mText;
        lastTextChanged->mDuration = uam->timeStamp() - lastAction->timeStamp();
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
}
