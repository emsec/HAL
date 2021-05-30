#include "gui/user_action/action_python_new_tab.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonNewTabFactory::ActionPythonNewTabFactory()
       : UserActionFactory("PythonNewTab") {;}

    ActionPythonNewTabFactory* ActionPythonNewTabFactory::sFactory = new ActionPythonNewTabFactory;

    UserAction* ActionPythonNewTabFactory::newAction() const
    {
        return new ActionPythonNewTab;
    }

    QString ActionPythonNewTab::tagname() const
    {
        return ActionPythonNewTabFactory::sFactory->tagname();
    }

    ActionPythonNewTab::ActionPythonNewTab(const u32 &id_)
        : mPythonCodeEditorId(id_)
    {
        if (id_)
            setObject(UserActionObject(id_,UserActionObjectType::PythonCodeEditor));
    }

    bool ActionPythonNewTab::exec()
    {
        u32 pythonCodeEditorId = gContentManager->getPythonEditorWidget()->newTab(mPythonCodeEditorId);
        mPythonCodeEditorId = pythonCodeEditorId;
        return UserAction::exec();
    }

    void ActionPythonNewTab::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(QByteArray::number(mPythonCodeEditorId));
    }

    void ActionPythonNewTab::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("uid", QString::number(mPythonCodeEditorId));
    }

    void ActionPythonNewTab::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "uid")
                mPythonCodeEditorId = xmlIn.readElementText().toInt();
        }
    }
}
