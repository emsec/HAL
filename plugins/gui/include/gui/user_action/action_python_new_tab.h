#pragma once
#include "user_action.h"

namespace hal
{
    /**
     * @ingroup user_action
     */
    class ActionPythonNewTab : public UserAction
    {
    public:
        ActionPythonNewTab();
        QString tagname() const override;
        bool exec() override;
    };

    /**
     * @ingroup user_action
     */
    class ActionPythonNewTabFactory : public UserActionFactory
    {
    public:
        ActionPythonNewTabFactory();
        UserAction* newAction() const;
        static ActionPythonNewTabFactory* sFactory;
    };
}
