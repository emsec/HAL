#include "gui/user_action/action_python_save_file.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonSaveFileFactory::ActionPythonSaveFileFactory()
       : UserActionFactory("SavePythonFile") {;}

    ActionPythonSaveFileFactory* ActionPythonSaveFileFactory::sFactory = new ActionPythonSaveFileFactory;

    UserAction* ActionPythonSaveFileFactory::newAction() const
    {
        return new ActionPythonSaveFile;
    }

    QString ActionPythonSaveFile::tagname() const
    {
        return ActionPythonSaveFileFactory::sFactory->tagname();
    }

    ActionPythonSaveFile::ActionPythonSaveFile(const u32 &tabIndex_, const QString &filename_)
        : mTabIndex(tabIndex_), mFilename(filename_)
    {
        mWaitForReady = true;
    }

    bool ActionPythonSaveFile::exec()
    {
        if(!gContentManager->getPythonEditorWidget()->execSaveFile(mTabIndex, mFilename)) return false;
        return UserAction::exec();
    }

    void ActionPythonSaveFile::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(mFilename.toUtf8());
    }

    void ActionPythonSaveFile::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("tabIndex", QString(mTabIndex));
        xmlOut.writeTextElement("filename", mFilename);
    }

    void ActionPythonSaveFile::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "filename")
                mFilename = xmlIn.readElementText();
            if (xmlIn.name() == "tabIndex")
                mTabIndex = xmlIn.readElementText().toInt();
        }
    }
}
