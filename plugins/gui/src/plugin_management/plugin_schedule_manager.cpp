#include "gui/plugin_management/plugin_schedule_manager.h"

#include "core/plugin_interface_cli.h"
#include "core/plugin_manager.h"
#include "core/program_arguments.h"
#include "gui/plugin_access_manager/plugin_access_manager.h"
#include "gui/plugin_management/plugin_arguments_widget.h"

namespace hal
{
    PluginScheduleManager* PluginScheduleManager::get_instance()
    {
        static PluginScheduleManager* manager = new PluginScheduleManager();
        return manager;

        // IMPLEMENT INDEX UPDATES (OR CHANGE ARCHITECTURE)
    }

    PluginScheduleManager::schedule* PluginScheduleManager::get_schedule()
    {
        return &m_schedule;
    }

    ProgramArguments PluginScheduleManager::get_program_arguments(int index)
    {
        ProgramArguments args;

        for (argument arg : m_schedule.at(index).second)
        {
            std::vector<std::string> vector;

            for (QString& qstring : arg.value.split(QRegExp("[\\s,;]*"), QString::SkipEmptyParts))
                vector.push_back(qstring.toStdString());

            if (arg.checked)
                args.set_option(arg.flag.toStdString(), vector);
        }
        return args;
    }

    int PluginScheduleManager::get_current_index()
    {
        return m_current_index;
    }

    void PluginScheduleManager::set_current_index(int index)
    {
        m_current_index = index;
    }

    void PluginScheduleManager::add_plugin(const QString& plugin, int index)
    {
        auto cli = plugin_manager::get_plugin_instance<CLIPluginInterface>(plugin.toStdString(), false);

        if (!cli)
            return;

        QList<argument> list;

        for (auto option_tupel : cli->get_cli_options().get_options())
        {
            argument arg;
            arg.flag        = QString::fromStdString(*std::get<0>(option_tupel).begin());
            arg.description = QString::fromStdString(std::get<1>(option_tupel));
            arg.value       = "";
            arg.checked     = false;
            list.append(arg);
        }

        m_schedule.insert(index, QPair<QString, QList<argument>>(plugin, list));
    }

    void PluginScheduleManager::move_plugin(int from, int to)
    {
        m_schedule.move(from, to);
    }

    void PluginScheduleManager::remove_plugin(int index)
    {
        m_schedule.removeAt(index);
    }

    void PluginScheduleManager::run_schedule()
    {
        for (int i = 0; i < m_schedule.length(); i++)
        {
            ProgramArguments args = get_program_arguments(i);
            plugin_access_manager::run_plugin(m_schedule.at(i).first.toStdString(), &args);
        }
    }

    PluginScheduleManager::PluginScheduleManager(QObject* parent) : QObject(parent), m_current_index(0)
    {
    }
}
