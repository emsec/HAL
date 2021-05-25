#include "gui/user_action/action_python_open_file.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonOpenFileFactory::ActionPythonOpenFileFactory()
       : UserActionFactory("OpenPythonFile") {;}

    ActionPythonOpenFileFactory* ActionPythonOpenFileFactory::sFactory = new ActionPythonOpenFileFactory;

    UserAction* ActionPythonOpenFileFactory::newAction() const
    {
        return new ActionPythonOpenFile;
    }

    QString ActionPythonOpenFile::tagname() const
    {
        return ActionPythonOpenFileFactory::sFactory->tagname();
    }

    ActionPythonOpenFile::ActionPythonOpenFile(const u32 &tabIndex_, const QString &filename_)
        : mTabIndex(tabIndex_), mFilename(filename_)
    {
        mWaitForReady = true;
    }

    bool ActionPythonOpenFile::exec()
    {
        gContentManager->getPythonEditorWidget()->tabLoadFile(mTabIndex, mFilename);
        return UserAction::exec();
    }

    void ActionPythonOpenFile::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(mFilename.toUtf8());
    }

    void ActionPythonOpenFile::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("filename", mFilename);
    }

    void ActionPythonOpenFile::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "filename")
                mFilename = xmlIn.readElementText();
        }
    }
}
