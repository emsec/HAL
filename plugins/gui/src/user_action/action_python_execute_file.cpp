#include "gui/user_action/action_python_execute_file.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonExecuteFileFactory::ActionPythonExecuteFileFactory()
       : UserActionFactory("OpenPythonFile") {;}

    ActionPythonExecuteFileFactory* ActionPythonExecuteFileFactory::sFactory = new ActionPythonExecuteFileFactory;

    UserAction* ActionPythonExecuteFileFactory::newAction() const
    {
        return new ActionPythonExecuteFile;
    }

    QString ActionPythonExecuteFile::tagname() const
    {
        return ActionPythonExecuteFileFactory::sFactory->tagname();
    }

    ActionPythonExecuteFile::ActionPythonExecuteFile()
    {
        mWaitForReady = true;
    }

    bool ActionPythonExecuteFile::exec()
    {
        gContentManager->getPythonEditorWidget()->runScript();
        return UserAction::exec();
    }

    void ActionPythonExecuteFile::addToHash(QCryptographicHash& cryptoHash) const
    {;}

    void ActionPythonExecuteFile::writeToXml(QXmlStreamWriter& xmlOut) const
    {;}

    void ActionPythonExecuteFile::readFromXml(QXmlStreamReader& xmlIn)
    {;}
}
