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

    ActionPythonNewTab::ActionPythonNewTab()
    {;}

    bool ActionPythonNewTab::exec()
    {
        gContentManager->getPythonEditorWidget()->newTab();
        return UserAction::exec();
    }
}
