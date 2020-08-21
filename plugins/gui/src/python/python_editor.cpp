#include "gui/python/python_editor.h"

#include "gui/code_editor/syntax_highlighter/python_syntax_highlighter.h"
#include "core/log.h"
#include "gui/graph_widget/contexts/graph_context.h"
#include "gui/gui_globals.h"
#include "gui/gui_utils/graphics.h"
#include "gui/action/action.h"
#include "gui/python/python_code_editor.h"
#include "gui/python/python_editor_code_completion_dialog.h"
#include "gui/searchbar/searchbar.h"
#include "gui/splitter/splitter.h"
#include "gui/toolbar/toolbar.h"

#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QShortcut>
#include <QTabBar>
#include <QTextDocumentFragment>
#include <QTextStream>
#include <QToolButton>
#include <QVBoxLayout>
#include <chrono>
#include <fstream>

namespace hal
{
    PythonEditor::PythonEditor(QWidget* parent)
        : ContentWidget("Python Editor", parent), PythonContextSubscriber(), m_searchbar(new Searchbar()), m_action_open_file(new Action(this)), m_action_run(new Action(this)),
          m_action_save(new Action(this)), m_action_save_as(new Action(this)), m_action_toggle_minimap(new Action(this)), m_action_new_file(new Action(this))
    {
        ensurePolished();
        m_new_file_counter = 0;
        m_last_click_time  = 0;

        m_last_opened_path = QDir::currentPath();

        m_tab_widget = new QTabWidget(this);
        m_tab_widget->setTabsClosable(true);
        m_tab_widget->setMovable(true);
        // we need to grab mouse events from the tab bar
        m_tab_widget->tabBar()->installEventFilter(this);
        m_content_layout->addWidget(m_tab_widget);
        connect(m_tab_widget, &QTabWidget::tabCloseRequested, this, &PythonEditor::handle_tab_close_requested);
        m_content_layout->addWidget(m_searchbar);
        m_searchbar->hide();

        m_action_open_file->setIcon(gui_utility::get_styled_svg_icon(m_open_icon_style, m_open_icon_path));
        m_action_save->setIcon(gui_utility::get_styled_svg_icon(m_save_icon_style, m_save_icon_path));
        m_action_save_as->setIcon(gui_utility::get_styled_svg_icon(m_save_as_icon_style, m_save_as_icon_path));
        m_action_run->setIcon(gui_utility::get_styled_svg_icon(m_run_icon_style, m_run_icon_path));
        m_action_toggle_minimap->setIcon(gui_utility::get_styled_svg_icon(m_toggle_minimap_icon_style, m_toggle_minimap_icon_path));
        m_action_new_file->setIcon(gui_utility::get_styled_svg_icon(m_new_file_icon_style, m_new_file_icon_path));

        g_keybind_manager.bind(m_action_open_file, "keybinds/python_open_file");
        g_keybind_manager.bind(m_action_save, "keybinds/python_save_file");
        g_keybind_manager.bind(m_action_save_as, "keybinds/python_save_file_as");
        g_keybind_manager.bind(m_action_run, "keybinds/python_run_file");
        g_keybind_manager.bind(m_action_new_file, "keybinds/python_create_file");

        m_action_open_file->setText("Open Script");
        m_action_save->setText("Save");
        m_action_save_as->setText("Save as");
        m_action_run->setText("Execute Script");
        m_action_new_file->setText("New File");
        m_action_toggle_minimap->setText("Toggle Minimap");

        connect(m_action_open_file, &Action::triggered, this, &PythonEditor::handle_action_open_file);
        connect(m_action_save, &Action::triggered, this, &PythonEditor::handle_action_save_file);
        connect(m_action_save_as, &Action::triggered, this, &PythonEditor::handle_action_save_file_as);
        connect(m_action_run, &Action::triggered, this, &PythonEditor::handle_action_run);
        connect(m_action_new_file, &Action::triggered, this, &PythonEditor::handle_action_new_tab);
        connect(m_action_toggle_minimap, &Action::triggered, this, &PythonEditor::handle_action_toggle_minimap);

        connect(m_searchbar, &Searchbar::text_edited, this, &PythonEditor::handle_searchbar_text_edited);
        connect(m_tab_widget, &QTabWidget::currentChanged, this, &PythonEditor::handle_current_tab_changed);

        m_path_editor_map = QMap<QString, PythonCodeEditor*>();

        m_file_modified_bar = new FileModifiedBar();
        m_file_modified_bar->setHidden(true);
        m_content_layout->addWidget(m_file_modified_bar);
        connect(m_file_modified_bar, &FileModifiedBar::reload_clicked, this, &PythonEditor::handle_base_file_modified_reload);
        connect(m_file_modified_bar, &FileModifiedBar::ignore_clicked, this, &PythonEditor::handle_base_file_modified_ignore);
        connect(m_file_modified_bar, &FileModifiedBar::ok_clicked, this, &PythonEditor::handle_base_file_modified_ok);

        m_file_watcher = new QFileSystemWatcher(this);
        connect(m_file_watcher, &QFileSystemWatcher::fileChanged, this, &PythonEditor::handle_tab_file_changed);
        connect(m_file_watcher, &QFileSystemWatcher::fileChanged, m_file_modified_bar, &FileModifiedBar::handle_file_changed);

        handle_action_new_tab();

        using namespace std::placeholders;
        hal_file_manager::register_on_serialize_callback("PythonEditor", std::bind(&PythonEditor::handle_serialization_to_hal_file, this, _1, _2, _3));
        hal_file_manager::register_on_deserialize_callback("PythonEditor", std::bind(&PythonEditor::handle_deserialization_from_hal_file, this, _1, _2, _3));
    }

    bool PythonEditor::handle_serialization_to_hal_file(const std::filesystem::path& path, Netlist* netlist, rapidjson::Document& document)
    {
        UNUSED(path);
        UNUSED(netlist);

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value tabs(rapidjson::kArrayType);

        for (int i = 0; i < m_tab_widget->count(); i++)
        {
            rapidjson::Value val(rapidjson::kObjectType);

            auto tab = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(i));

            if (!tab->get_file_name().isEmpty())
            {
                val.AddMember("path", tab->get_file_name().toStdString(), allocator);
            }

            if (tab->document()->isModified())
            {
                val.AddMember("script", tab->toPlainText().toStdString(), allocator);
            }

            tabs.PushBack(val, allocator);
        }

        if (!tabs.Empty())
        {
            rapidjson::Value root(rapidjson::kObjectType);
            root.AddMember("tabs", tabs, allocator);
            if (m_tab_widget->currentIndex() != -1)
            {
                root.AddMember("selected_tab", rapidjson::Value(m_tab_widget->currentIndex()), allocator);
            }
            document.AddMember("PythonEditor", root, allocator);
        }
        return true;
    }

    bool PythonEditor::handle_deserialization_from_hal_file(const std::filesystem::path& path, Netlist* netlist, rapidjson::Document& document)
    {
        UNUSED(path);
        UNUSED(netlist);

        if (document.HasMember("PythonEditor"))
        {
            auto root  = document["PythonEditor"].GetObject();
            int cnt    = 0;
            auto array = root["tabs"].GetArray();
            for (auto it = array.Begin(); it != array.End(); ++it)
            {
                cnt++;
                if (m_tab_widget->count() < cnt)
                {
                    handle_action_new_tab();
                }

                auto val = it->GetObject();

                if (val.HasMember("path"))
                {
                    tab_load_file(cnt - 1, val["path"].GetString());
                }

                if (val.HasMember("script"))
                {
                    auto tab = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(cnt - 1));
                    tab->setPlainText(val["script"].GetString());
                    tab->document()->setModified(true);
                    m_tab_widget->setTabText(cnt - 1, m_tab_widget->tabText(cnt - 1) + "*");
                }
            }

            if (root.HasMember("selected_tab"))
            {
                m_tab_widget->setCurrentIndex(root["selected_tab"].GetInt());
            }
        }
        return true;
    }

    void PythonEditor::handle_tab_close_requested(int index)
    {
        PythonCodeEditor* editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(index));
        if (editor->document()->isModified())
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("QLabel{min-width: 600px;}");
            msgBox.setText(m_tab_widget->tabText(index).append(" has been modified."));
            msgBox.setInformativeText("Do you want to save your changes?");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Save);
            int ret = msgBox.exec();

            if (ret == QMessageBox::Cancel)
                return;

            // discard is not handled specially, we just treat the document
            // as if it did not require saving and call discard_tab on it

            if (ret == QMessageBox::Save)
            {
                if (editor->get_file_name().isEmpty())
                    save_file(true, index);
                else
                    save_file(false, index);

                m_tab_widget->removeTab(index);
                return;
            }
        }

        this->discard_tab(index);
    }

    void PythonEditor::handle_action_toggle_minimap()
    {
        if (m_tab_widget->currentWidget())
            dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget())->toggle_minimap();
    }

    void PythonEditor::handle_modification_changed(bool changed)
    {
        if (changed && !(m_tab_widget->tabText(m_tab_widget->currentIndex()).endsWith("*")))
        {
            m_tab_widget->setTabText(m_tab_widget->currentIndex(), m_tab_widget->tabText(m_tab_widget->currentIndex()).append("*"));
        }
        if (!changed && (m_tab_widget->tabText(m_tab_widget->currentIndex()).endsWith("*")))
        {
            m_tab_widget->setTabText(m_tab_widget->currentIndex(), m_tab_widget->tabText(m_tab_widget->currentIndex()).remove('*'));
        }
    }

    void PythonEditor::handle_key_pressed()
    {
        m_last_click_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void PythonEditor::handle_text_changed()
    {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - 100 < m_last_click_time)
        {
            PythonCodeEditor* current_editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget());

            QString tab_name = m_tab_widget->tabText(m_tab_widget->indexOf(current_editor));

            if (current_editor)
                g_file_status_manager.file_changed(current_editor->get_uuid(), "Python tab: " + tab_name);

            if (!tab_name.endsWith("*"))
                m_tab_widget->setTabText(m_tab_widget->indexOf(current_editor), tab_name + "*");
        }
    }

    void PythonEditor::handle_searchbar_text_edited(const QString& text)
    {
        if (m_tab_widget->count() > 0)
            dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget())->search(text);
    }

    void PythonEditor::handle_current_tab_changed(int index)
    {
        Q_UNUSED(index)

        if (!m_tab_widget->currentWidget())
            return;

        PythonCodeEditor* current_editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget());

        if (!m_searchbar->isHidden())
            current_editor->search(m_searchbar->get_current_text());
        else if (!current_editor->extraSelections().isEmpty())
            current_editor->search("");

        if (current_editor->is_base_file_modified())
            m_file_modified_bar->setHidden(false);
        else
            m_file_modified_bar->setHidden(true);
    }

    PythonEditor::~PythonEditor()
    {
        hal_file_manager::unregister_on_serialize_callback("PythonEditor");
        hal_file_manager::unregister_on_deserialize_callback("PythonEditor");
    }

    void PythonEditor::setup_toolbar(Toolbar* Toolbar)
    {
        Toolbar->addAction(m_action_new_file);
        Toolbar->addAction(m_action_open_file);
        Toolbar->addAction(m_action_save);
        Toolbar->addAction(m_action_save_as);
        Toolbar->addAction(m_action_run);
        Toolbar->addAction(m_action_toggle_minimap);
    }

    QList<QShortcut*> PythonEditor::create_shortcuts()
    {
        QShortcut* search_shortcut = new QShortcut(QKeySequence("Ctrl+f"), this);
        connect(search_shortcut, &QShortcut::activated, this, &PythonEditor::toggle_searchbar);

        QList<QShortcut*> list;
        list.append(search_shortcut);

        return list;
    }

    void PythonEditor::handle_stdout(const QString& output)
    {
        Q_EMIT forward_stdout(output);
    }

    void PythonEditor::handle_error(const QString& output)
    {
        Q_EMIT forward_error(output);
    }

    void PythonEditor::clear()
    {
    }

    void PythonEditor::handle_action_open_file()
    {
        QString title = "Open File";
        QString text  = "Python Scripts(*.py)";

        // Non native dialogs does not work on macOS. Therefore do net set DontUseNativeDialog!
        QStringList file_names = QFileDialog::getOpenFileNames(nullptr, title, m_last_opened_path, text, nullptr);

        if (file_names.isEmpty())
        {
            return;
        }

        for (auto file_name : file_names)
        {
            for (int i = 0; i < m_tab_widget->count(); ++i)
            {
                auto editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(i));
                if (editor->get_file_name() == file_name)
                {
                    m_tab_widget->setCurrentIndex(i);

                    if (editor->document()->isModified())
                    {
                        if (QMessageBox::question(editor, "Script has unsaved changes", "Do you want to reload the file from disk? Unsaved changes are lost.", QMessageBox::Yes | QMessageBox::No)
                            == QMessageBox::Yes)
                        {
                            tab_load_file(i, file_name);
                        }
                    }
                    return;
                }
            }

            handle_action_new_tab();
            tab_load_file(m_tab_widget->count() - 1, file_name);
        }

        m_last_opened_path = QFileInfo(file_names.last()).absolutePath();
    }

    void PythonEditor::tab_load_file(u32 index, QString file_name)
    {
        std::ifstream file(file_name.toStdString(), std::ios::in);

        if (!file.is_open())
        {
            return;
        }

        std::string f((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        QFileInfo info(file_name);

        auto tab = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(index));

        tab->setPlainText(QString::fromStdString(f));
        tab->set_file_name(file_name);
        tab->document()->setModified(false);
        //m_tab_widget->setTabText(m_tab_widget->count() - 1, info.completeBaseName() + "." + info.completeSuffix());
        m_tab_widget->setTabText(m_tab_widget->indexOf(tab), info.completeBaseName() + "." + info.completeSuffix());
        m_new_file_counter--;

        m_path_editor_map.insert(file_name, tab);
        m_file_watcher->addPath(file_name);

        g_file_status_manager.file_saved(tab->get_uuid());
    }

    void PythonEditor::save_file(const bool ask_path, int index)
    {
        QString title = "Save File";
        QString text  = "Python Scripts(*.py)";

        QString selected_file_name;

        if (index == -1)
        {
            index = m_tab_widget->currentIndex();
        }

        PythonCodeEditor* current_editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(index));

        if (!current_editor)
            return;

        if (ask_path || current_editor->get_file_name().isEmpty())
        {
            selected_file_name = QFileDialog::getSaveFileName(nullptr, title, m_last_opened_path, text, nullptr, QFileDialog::DontUseNativeDialog);
            if (selected_file_name.isEmpty())
                return;

            if (!selected_file_name.endsWith(".py"))
                selected_file_name.append(".py");

            current_editor->set_file_name(selected_file_name);
            m_last_opened_path = selected_file_name;
        }
        else
        {
            selected_file_name = current_editor->get_file_name();
        }

        m_file_watcher->removePath(current_editor->get_file_name());
        m_path_editor_map.remove(current_editor->get_file_name());

        std::ofstream out(selected_file_name.toStdString(), std::ios::out);

        if (!out.is_open())
        {
            log_error("gui", "could not open file path");
            return;
        }
        out << current_editor->toPlainText().toStdString();
        out.close();
        current_editor->document()->setModified(false);

        g_file_status_manager.file_saved(current_editor->get_uuid());

        m_path_editor_map.insert(selected_file_name, current_editor);
        m_file_watcher->addPath(selected_file_name);

        QFileInfo info(selected_file_name);
        m_tab_widget->setTabText(index, info.completeBaseName() + "." + info.completeSuffix());
    }

    void PythonEditor::discard_tab(int index)
    {
        PythonCodeEditor* editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(index));
        QString s                  = editor->get_file_name();
        if (!s.isEmpty())
        {
            m_file_watcher->removePath(s);
            m_path_editor_map.remove(s);
        }
        if (editor->document()->isModified())
        {
            g_file_status_manager.file_saved(editor->get_uuid());
        }
        m_tab_widget->removeTab(index);
    }

    bool PythonEditor::confirm_discard_for_range(int start, int end, int exclude)
    {
        QString changedFiles = "The following files have not been saved yet:\n";
        int unsaved          = 0;
        int total            = end - start - (exclude == -1 ? 0 : 1);
        for (int t = start; t < end; t++)
        {
            // to disable, set exclude=-1
            if (t == exclude)
                continue;

            PythonCodeEditor* editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(t));
            if (editor->document()->isModified())
            {
                QString fileName = m_tab_widget->tabText(t);
                fileName.chop(1);    // removes asterisk
                changedFiles.append("   ->  " + fileName + "\n");
                unsaved++;
            }
        }
        if (unsaved)
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("QLabel{min-width: 600px;}");
            auto cancelButton = msgBox.addButton("Cancel", QMessageBox::RejectRole);
            msgBox.addButton("Close Anyway", QMessageBox::ApplyRole);
            msgBox.setDefaultButton(cancelButton);
            msgBox.setInformativeText(QStringLiteral("Are you sure you want to close %1 tabs, %2 unsaved, anyway?").arg(total).arg(unsaved));
            msgBox.setText("There are unsaved modifications that will be lost.");
            msgBox.setDetailedText(changedFiles);

            msgBox.exec();
            if (msgBox.clickedButton() == cancelButton)
            {
                return false;
            }
        }
        return true;
    }

    void PythonEditor::handle_action_save_file()
    {
        this->save_file(false);
    }

    void PythonEditor::handle_action_save_file_as()
    {
        this->save_file(true);
    }

    void PythonEditor::handle_action_run()
    {
        for (const auto& ctx : g_graph_context_manager.get_contexts())
        {
            ctx->begin_change();
        }

        g_python_context->interpret_script(dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget())->toPlainText());

        for (const auto& ctx : g_graph_context_manager.get_contexts())
        {
            ctx->end_change();
        }
    }

    void PythonEditor::handle_action_new_tab()
    {
        PythonCodeEditor* editor = new PythonCodeEditor();
        new PythonSyntaxHighlighter(editor->document());
        new PythonSyntaxHighlighter(editor->minimap()->document());
        m_tab_widget->addTab(editor, QString("New File ").append(QString::number(++m_new_file_counter)));
        m_tab_widget->setCurrentIndex(m_tab_widget->count() - 1);
        editor->document()->setModified(false);
        //connect(editor, &PythonCodeEditor::modificationChanged, this, &PythonEditor::handle_modification_changed);
        connect(editor, &PythonCodeEditor::key_pressed, this, &PythonEditor::handle_key_pressed);
        connect(editor, &PythonCodeEditor::textChanged, this, &PythonEditor::handle_text_changed);
    }

    void PythonEditor::handle_action_tab_menu()
    {
        QMenu context_menu(this);
        QAction* action = context_menu.addAction("Close");

        context_menu.addSeparator();
        connect(action, &QAction::triggered, this, &PythonEditor::handle_action_close_tab);
        action = context_menu.addAction("Close all");
        connect(action, &QAction::triggered, this, &PythonEditor::handle_action_close_all_tabs);
        action = context_menu.addAction("Close all others");
        connect(action, &QAction::triggered, this, &PythonEditor::handle_action_close_other_tabs);
        action = context_menu.addAction("Close all right");
        connect(action, &QAction::triggered, this, &PythonEditor::handle_action_close_right_tabs);
        action = context_menu.addAction("Close all left");
        connect(action, &QAction::triggered, this, &PythonEditor::handle_action_close_left_tabs);

        context_menu.addSeparator();
        action                     = context_menu.addAction("Show in system explorer");
        PythonCodeEditor* editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->widget(m_tab_rightclicked));
        QString s                  = editor->get_file_name();
        action->setDisabled(s.isEmpty());
        connect(action, &QAction::triggered, this, &PythonEditor::handle_action_show_file);

        context_menu.exec(QCursor::pos());
    }

    void PythonEditor::handle_action_close_tab()
    {
        assert(m_tab_rightclicked != -1);
        this->handle_tab_close_requested(m_tab_rightclicked);
    }

    void PythonEditor::handle_action_close_all_tabs()
    {
        assert(m_tab_rightclicked != -1);
        int tabs = m_tab_widget->count();
        if (!this->confirm_discard_for_range(0, tabs))
            return;
        for (int t = 0; t < tabs; t++)
        {
            this->discard_tab(0);
        }
    }

    void PythonEditor::handle_action_close_other_tabs()
    {
        assert(m_tab_rightclicked != -1);
        int tabs = m_tab_widget->count();
        if (!this->confirm_discard_for_range(0, tabs, m_tab_rightclicked))
            return;
        int discard_id = 0;    // keeps track of IDs shifting during deletion
        for (int t = 0; t < tabs; t++)
        {
            // don't close the right-clicked tab
            if (t == m_tab_rightclicked)
            {
                discard_id++;
                continue;
            }
            this->discard_tab(discard_id);
        }
    }

    void PythonEditor::handle_action_close_left_tabs()
    {
        assert(m_tab_rightclicked != -1);
        if (!this->confirm_discard_for_range(0, m_tab_rightclicked, -1))
            return;
        for (int t = 0; t < m_tab_rightclicked; t++)
        {
            // IDs shift downwards during deletion
            this->discard_tab(0);
        }
    }

    void PythonEditor::handle_action_close_right_tabs()
    {
        assert(m_tab_rightclicked != -1);
        int tabs = m_tab_widget->count();
        if (!this->confirm_discard_for_range(m_tab_rightclicked + 1, tabs, -1))
            return;
        for (int t = m_tab_rightclicked + 1; t < tabs; t++)
        {
            // IDs shift downwards during deletion
            this->discard_tab(m_tab_rightclicked + 1);
        }
    }

    void PythonEditor::handle_action_show_file()
    {
    }

    void PythonEditor::handle_tab_file_changed(QString path)
    {
        PythonCodeEditor* editor_with_modified_base_file = m_path_editor_map.value(path);
        editor_with_modified_base_file->set_base_file_modified(true);
        QString tab_name = m_tab_widget->tabText(m_tab_widget->indexOf(editor_with_modified_base_file));

        if (!tab_name.endsWith("*"))
            m_tab_widget->setTabText(m_tab_widget->indexOf(editor_with_modified_base_file), tab_name + "*");

        g_file_status_manager.file_changed(editor_with_modified_base_file->get_uuid(), "Python tab: " + tab_name);

        PythonCodeEditor* current_editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget());

        if (editor_with_modified_base_file == current_editor)
            m_file_modified_bar->setHidden(false);

        m_file_watcher->addPath(path);
    }

    void PythonEditor::handle_base_file_modified_reload()
    {
        PythonCodeEditor* current_editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget());
        m_new_file_counter++;
        //tab_load_file(current_editor, current_editor->get_file_name());
        tab_load_file(m_tab_widget->indexOf(current_editor), current_editor->get_file_name());
        current_editor->set_base_file_modified(false);
        m_file_modified_bar->setHidden(true);
    }

    void PythonEditor::handle_base_file_modified_ignore()
    {
        PythonCodeEditor* current_editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget());
        current_editor->set_base_file_modified(false);
        m_file_modified_bar->setHidden(true);
    }

    void PythonEditor::handle_base_file_modified_ok()
    {
        PythonCodeEditor* current_editor = dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget());
        current_editor->set_base_file_modified(false);
        m_file_modified_bar->setHidden(true);
    }

    bool PythonEditor::eventFilter(QObject* obj, QEvent* event)
    {
        if (obj == m_tab_widget->tabBar() && event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            // filter for right-mouse-button-pressed events
            if (mouseEvent->button() == Qt::MouseButton::RightButton)
            {
                m_tab_rightclicked = m_tab_widget->tabBar()->tabAt(mouseEvent->pos());
                this->handle_action_tab_menu();
                return true;
            }
        }
        // otherwise honor default filter
        return QObject::eventFilter(obj, event);
    }

    QString PythonEditor::open_icon_path() const
    {
        return m_open_icon_path;
    }

    QString PythonEditor::open_icon_style() const
    {
        return m_open_icon_style;
    }

    QString PythonEditor::save_icon_path() const
    {
        return m_save_icon_path;
    }

    QString PythonEditor::save_icon_style() const
    {
        return m_save_icon_style;
    }

    QString PythonEditor::save_as_icon_path() const
    {
        return m_save_as_icon_path;
    }

    QString PythonEditor::save_as_icon_style() const
    {
        return m_save_as_icon_style;
    }

    QString PythonEditor::run_icon_path() const
    {
        return m_run_icon_path;
    }

    QString PythonEditor::run_icon_style() const
    {
        return m_run_icon_style;
    }

    QString PythonEditor::new_file_icon_path() const
    {
        return m_new_file_icon_path;
    }

    QString PythonEditor::new_file_icon_style() const
    {
        return m_new_file_icon_style;
    }

    QString PythonEditor::toggle_minimap_icon_path() const
    {
        return m_toggle_minimap_icon_path;
    }

    QString PythonEditor::toggle_minimap_icon_style() const
    {
        return m_toggle_minimap_icon_style;
    }

    void PythonEditor::set_open_icon_path(const QString& path)
    {
        m_open_icon_path = path;
    }

    void PythonEditor::set_open_icon_style(const QString& style)
    {
        m_open_icon_style = style;
    }

    void PythonEditor::set_save_icon_path(const QString& path)
    {
        m_save_icon_path = path;
    }

    void PythonEditor::set_save_icon_style(const QString& style)
    {
        m_save_icon_style = style;
    }

    void PythonEditor::set_save_as_icon_path(const QString& path)
    {
        m_save_as_icon_path = path;
    }

    void PythonEditor::set_save_as_icon_style(const QString& style)
    {
        m_save_as_icon_style = style;
    }

    void PythonEditor::set_run_icon_path(const QString& path)
    {
        m_run_icon_path = path;
    }

    void PythonEditor::set_run_icon_style(const QString& style)
    {
        m_run_icon_style = style;
    }

    void PythonEditor::set_new_file_icon_path(const QString& path)
    {
        m_new_file_icon_path = path;
    }

    void PythonEditor::set_new_file_icon_style(const QString& style)
    {
        m_new_file_icon_style = style;
    }

    void PythonEditor::set_toggle_minimap_icon_path(const QString& path)
    {
        m_toggle_minimap_icon_path = path;
    }

    void PythonEditor::set_toggle_minimap_icon_style(const QString& style)
    {
        m_toggle_minimap_icon_style = style;
    }

    void PythonEditor::toggle_searchbar()
    {
        if (m_searchbar->isHidden())
        {
            m_searchbar->show();
            if (m_tab_widget->currentWidget())
                dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget())->search(m_searchbar->get_current_text());
            m_searchbar->setFocus();
        }
        else
        {
            m_searchbar->hide();
            if (m_tab_widget->currentWidget())
            {
                dynamic_cast<PythonCodeEditor*>(m_tab_widget->currentWidget())->search("");
                m_tab_widget->currentWidget()->setFocus();
            }
            else
                this->setFocus();
        }
    }
}
