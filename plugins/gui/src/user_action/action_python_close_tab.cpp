#include "gui/user_action/action_python_close_tab.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonCloseTabFactory::ActionPythonCloseTabFactory()
       : UserActionFactory("PythonCloseTab") {;}

    ActionPythonCloseTabFactory* ActionPythonCloseTabFactory::sFactory = new ActionPythonCloseTabFactory;

    UserAction* ActionPythonCloseTabFactory::newAction() const
    {
        return new ActionPythonCloseTab;
    }

    QString ActionPythonCloseTab::tagname() const
    {
        return ActionPythonCloseTabFactory::sFactory->tagname();
    }

    ActionPythonCloseTab::ActionPythonCloseTab(const u32 &id_)
        : mPythonCodeEditorId(id_)
    {
        if (id_)
            setObject(UserActionObject(id_,UserActionObjectType::PythonCodeEditor));
    }

    bool ActionPythonCloseTab::exec()
    {
        gContentManager->getPythonEditorWidget()->execDiscardTab(mPythonCodeEditorId);
        return UserAction::exec();
    }

    void ActionPythonCloseTab::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(QByteArray::number(mPythonCodeEditorId));
    }

    void ActionPythonCloseTab::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("uid", QString::number(mPythonCodeEditorId));
    }

    void ActionPythonCloseTab::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "uid")
                mPythonCodeEditorId = xmlIn.readElementText().toInt();
        }
    }
}
