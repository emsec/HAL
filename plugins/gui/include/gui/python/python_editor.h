//  MIT License
//
//  Copyright (c) 2019 Ruhr-University Bochum, Germany, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2019 Marc Fyrbiak, Sebastian Wallat, Max Hoffmann ("ORIGINAL AUTHORS"). All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include "gui/code_editor/code_editor.h"
#include "gui/content_widget/content_widget.h"
#include "core/hal_file_manager.h"
#include "gui/file_modified_bar/file_modified_bar.h"
#include "gui/action/action.h"
#include "gui/python/python_context_subscriber.h"

#include <QEvent>
#include <QFileSystemWatcher>
#include <QMap>
#include <QPushButton>

class QVBoxLayout;
class QTabWidget;

namespace hal
{
    class CodeEditor;
    class Searchbar;
    class Splitter;
    class Toolbar;

    class PythonCodeEditor;

    class PythonEditor : public ContentWidget, public PythonContextSubscriber
    {
        Q_OBJECT
        Q_PROPERTY(QString open_icon_path READ open_icon_path WRITE set_open_icon_path)
        Q_PROPERTY(QString open_icon_style READ open_icon_style WRITE set_open_icon_style)
        Q_PROPERTY(QString save_icon_path READ save_icon_path WRITE set_save_icon_path)
        Q_PROPERTY(QString save_icon_style READ save_icon_style WRITE set_save_icon_style)
        Q_PROPERTY(QString save_as_icon_path READ save_as_icon_path WRITE set_save_as_icon_path)
        Q_PROPERTY(QString save_as_icon_style READ save_as_icon_style WRITE set_save_as_icon_style)
        Q_PROPERTY(QString run_icon_path READ run_icon_path WRITE set_run_icon_path)
        Q_PROPERTY(QString run_icon_style READ run_icon_style WRITE set_run_icon_style)
        Q_PROPERTY(QString new_file_icon_path READ new_file_icon_path WRITE set_new_file_icon_path)
        Q_PROPERTY(QString new_file_icon_style READ new_file_icon_style WRITE set_new_file_icon_style)
        Q_PROPERTY(QString toggle_minimap_icon_path READ toggle_minimap_icon_path WRITE set_toggle_minimap_icon_path)
        Q_PROPERTY(QString toggle_minimap_icon_style READ toggle_minimap_icon_style WRITE set_toggle_minimap_icon_style)

    public:
        explicit PythonEditor(QWidget* parent = nullptr);
        ~PythonEditor();

        virtual void setup_toolbar(Toolbar* Toolbar) Q_DECL_OVERRIDE;
        virtual QList<QShortcut*> create_shortcuts() Q_DECL_OVERRIDE;

        virtual void handle_stdout(const QString& output) Q_DECL_OVERRIDE;
        virtual void handle_error(const QString& output) Q_DECL_OVERRIDE;
        virtual void clear() Q_DECL_OVERRIDE;

        void handle_action_open_file();
        void handle_action_save_file();
        void handle_action_save_file_as();
        void handle_action_run();
        void handle_action_new_tab();
        void handle_action_tab_menu();
        void handle_action_close_tab();
        void handle_action_close_all_tabs();
        void handle_action_close_other_tabs();
        void handle_action_close_left_tabs();
        void handle_action_close_right_tabs();
        void handle_action_show_file();
        void tab_load_file(u32 index, QString file_name);

        void save_file(const bool ask_path, int index = -1);

        void discard_tab(int index);
        bool confirm_discard_for_range(int start, int end, int exclude = -1);

        QString open_icon_path() const;
        QString open_icon_style() const;

        QString save_icon_path() const;
        QString save_icon_style() const;

        QString save_as_icon_path() const;
        QString save_as_icon_style() const;

        QString run_icon_path() const;
        QString run_icon_style() const;

        QString new_file_icon_path() const;
        QString new_file_icon_style() const;

        QString toggle_minimap_icon_path() const;
        QString toggle_minimap_icon_style() const;

        void set_open_icon_path(const QString& path);
        void set_open_icon_style(const QString& style);

        void set_save_icon_path(const QString& path);
        void set_save_icon_style(const QString& style);

        void set_save_as_icon_path(const QString& path);
        void set_save_as_icon_style(const QString& style);

        void set_run_icon_path(const QString& path);
        void set_run_icon_style(const QString& style);

        void set_new_file_icon_path(const QString& path);
        void set_new_file_icon_style(const QString& style);

        void set_toggle_minimap_icon_path(const QString& path);
        void set_toggle_minimap_icon_style(const QString& style);

        bool handle_serialization_to_hal_file(const std::filesystem::path& path, Netlist* netlist, rapidjson::Document& document);
        bool handle_deserialization_from_hal_file(const std::filesystem::path& path, Netlist* netlist, rapidjson::Document& document);

    Q_SIGNALS:
        void forward_stdout(const QString& output);
        void forward_error(const QString& output);

    public Q_SLOTS:
        void toggle_searchbar();
        void handle_tab_close_requested(int index);
        void handle_action_toggle_minimap();
        void handle_modification_changed(bool changed);
        void handle_key_pressed();
        void handle_text_changed();
        void handle_searchbar_text_edited(const QString& text);
        void handle_current_tab_changed(int index);
        void handle_tab_file_changed(QString path);

        void handle_base_file_modified_reload();
        void handle_base_file_modified_ignore();
        void handle_base_file_modified_ok();

    protected:
        bool eventFilter(QObject* obj, QEvent* event) Q_DECL_OVERRIDE;

    private:
        QVBoxLayout* m_layout;
        Toolbar* m_toolbar;
        Splitter* m_splitter;

        Searchbar* m_searchbar;

        Action* m_action_open_file;
        Action* m_action_run;
        Action* m_action_save;
        Action* m_action_save_as;
        Action* m_action_toggle_minimap;
        Action* m_action_new_file;

        QString m_open_icon_style;
        QString m_open_icon_path;

        QString m_save_icon_style;
        QString m_save_icon_path;

        QString m_save_as_icon_style;
        QString m_save_as_icon_path;

        QString m_run_icon_style;
        QString m_run_icon_path;

        QString m_new_file_icon_style;
        QString m_new_file_icon_path;

        QString m_toggle_minimap_icon_style;
        QString m_toggle_minimap_icon_path;

        QTabWidget* m_tab_widget;
        int m_tab_rightclicked = -1;

        QFileSystemWatcher* m_file_watcher;
        QMap<QString, PythonCodeEditor*> m_path_editor_map;

        FileModifiedBar* m_file_modified_bar;

        int m_new_file_counter;

        long m_last_click_time;

        QString m_last_opened_path;
    };
}
