#include "gui/user_action/action_python_execute_file.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonExecuteFileFactory::ActionPythonExecuteFileFactory()
       : UserActionFactory("PythonExecuteFile") {;}

    ActionPythonExecuteFileFactory* ActionPythonExecuteFileFactory::sFactory = new ActionPythonExecuteFileFactory;

    UserAction* ActionPythonExecuteFileFactory::newAction() const
    {
        return new ActionPythonExecuteFile;
    }

    QString ActionPythonExecuteFile::tagname() const
    {
        return ActionPythonExecuteFileFactory::sFactory->tagname();
    }

    ActionPythonExecuteFile::ActionPythonExecuteFile(u32 id_)
        : mPythonCodeEditorId(id_)
    {
        mWaitForReady = true;

        if (id_)
            setObject(UserActionObject(id_,UserActionObjectType::PythonCodeEditor));
    }

    bool ActionPythonExecuteFile::exec()
    {
        gContentManager->getPythonEditorWidget()->runScript(mPythonCodeEditorId);
        return UserAction::exec();
    }

    void ActionPythonExecuteFile::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(QByteArray::number(mPythonCodeEditorId));
    }

    void ActionPythonExecuteFile::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("uid", QString::number(mPythonCodeEditorId));
    }

    void ActionPythonExecuteFile::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "uid")
                mPythonCodeEditorId = xmlIn.readElementText().toInt();
        }
    }
}
