#include "gui/user_action/action_python_new_tab.h"
#include "gui/gui_globals.h"
#include "gui/python/python_editor.h"

namespace hal
{
    ActionPythonNewTabFactory::ActionPythonNewTabFactory()
       : UserActionFactory("NewPythonTab") {;}

    ActionPythonNewTabFactory* ActionPythonNewTabFactory::sFactory = new ActionPythonNewTabFactory;

    UserAction* ActionPythonNewTabFactory::newAction() const
    {
        return new ActionPythonNewTab;
    }

    QString ActionPythonNewTab::tagname() const
    {
        return ActionPythonNewTabFactory::sFactory->tagname();
    }

    ActionPythonNewTab::ActionPythonNewTab()
    {;}

    bool ActionPythonNewTab::exec()
    {
        gContentManager->getPythonEditorWidget()->newTab();
        return UserAction::exec();
    }

    void ActionPythonNewTab::addToHash(QCryptographicHash& cryptoHash) const
    {;}

    void ActionPythonNewTab::writeToXml(QXmlStreamWriter& xmlOut) const
    {;}

    void ActionPythonNewTab::readFromXml(QXmlStreamReader& xmlIn)
    {;}
}
