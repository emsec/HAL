#include "gui/user_action/action_python_open_file.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonOpenFileFactory::ActionPythonOpenFileFactory()
       : UserActionFactory("PythonOpenFile") {;}

    ActionPythonOpenFileFactory* ActionPythonOpenFileFactory::sFactory = new ActionPythonOpenFileFactory;

    UserAction* ActionPythonOpenFileFactory::newAction() const
    {
        return new ActionPythonOpenFile;
    }

    QString ActionPythonOpenFile::tagname() const
    {
        return ActionPythonOpenFileFactory::sFactory->tagname();
    }

    ActionPythonOpenFile::ActionPythonOpenFile(const u32 &id_, const QString &filename_)
        : mPythonCodeEditorId(id_), mFilename(filename_)
    {
        mWaitForReady = true;

        if (id_)
            setObject(UserActionObject(id_,UserActionObjectType::PythonCodeEditor));
    }

    bool ActionPythonOpenFile::exec()
    {
        gContentManager->getPythonEditorWidget()->tabLoadFile(mPythonCodeEditorId, mFilename);
        return UserAction::exec();
    }

    void ActionPythonOpenFile::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(mFilename.toUtf8());
        cryptoHash.addData(QByteArray::number(mPythonCodeEditorId));
    }

    void ActionPythonOpenFile::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("filename", mFilename);
        xmlOut.writeTextElement("uid", QString::number(mPythonCodeEditorId));
    }

    void ActionPythonOpenFile::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "filename")
                mFilename = xmlIn.readElementText();
            if (xmlIn.name() == "uid")
                mPythonCodeEditorId = xmlIn.readElementText().toInt();
        }
    }
}
