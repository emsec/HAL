#include "gui/user_action/action_python_tab_changed.h"
#include <QTabWidget>
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonTabChangedFactory::ActionPythonTabChangedFactory()
       : UserActionFactory("PythonTabChanged") {;}

    ActionPythonTabChangedFactory* ActionPythonTabChangedFactory::sFactory = new ActionPythonTabChangedFactory;

    UserAction* ActionPythonTabChangedFactory::newAction() const
    {
        return new ActionPythonTabChanged;
    }

    QString ActionPythonTabChanged::tagname() const
    {
        return ActionPythonTabChangedFactory::sFactory->tagname();
    }

    ActionPythonTabChanged::ActionPythonTabChanged(const u32 &newIndex_)
        : mNewIndex(newIndex_)
    {;}

    bool ActionPythonTabChanged::exec()
    {
        if(!UserActionManager::instance()->isUserTriggeredAction()) {
            gContentManager->getPythonEditorWidget()->getTabWidget()->setCurrentIndex(mNewIndex);
        }
        return UserAction::exec();
    }

    void ActionPythonTabChanged::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(QByteArray::number(mNewIndex));
    }

    void ActionPythonTabChanged::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("index", QString::number(mNewIndex));
    }

    void ActionPythonTabChanged::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "index")
                mNewIndex = xmlIn.readElementText().toInt();
        }
    }
}
