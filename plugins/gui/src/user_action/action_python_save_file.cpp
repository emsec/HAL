#include "gui/user_action/action_python_save_file.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonSaveFileFactory::ActionPythonSaveFileFactory()
       : UserActionFactory("PythonSaveFile") {;}

    ActionPythonSaveFileFactory* ActionPythonSaveFileFactory::sFactory = new ActionPythonSaveFileFactory;

    UserAction* ActionPythonSaveFileFactory::newAction() const
    {
        return new ActionPythonSaveFile;
    }

    QString ActionPythonSaveFile::tagname() const
    {
        return ActionPythonSaveFileFactory::sFactory->tagname();
    }

    ActionPythonSaveFile::ActionPythonSaveFile(const u32 &id_, const QString &filename_)
        : mPythonCodeEditorId(id_), mFilename(filename_)
    {
        mWaitForReady = true;

        if (id_)
            setObject(UserActionObject(id_,UserActionObjectType::PythonCodeEditor));
    }

    bool ActionPythonSaveFile::exec()
    {
        if(!gContentManager->getPythonEditorWidget()->execSaveFile(mPythonCodeEditorId, mFilename)) return false;
        return UserAction::exec();
    }

    void ActionPythonSaveFile::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(mFilename.toUtf8());
        cryptoHash.addData(QByteArray::number(mPythonCodeEditorId));
    }

    void ActionPythonSaveFile::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("uid", QString::number(mPythonCodeEditorId));
        xmlOut.writeTextElement("filename", mFilename);
    }

    void ActionPythonSaveFile::readFromXml(QXmlStreamReader& xmlIn)
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
