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

#ifndef HAL_LOGGER_WIDGET_H
#define HAL_LOGGER_WIDGET_H

#include "channel_manager/channel_item.h"
#include "content_widget/content_widget.h"
#include "core/log.h"
#include "gui_utils/graphics.h"
#include <QLabel>
#include <QMenu>
#include <QPlainTextEdit>
#include <QString>
#include <QToolButton>
#include <QWidget>
#include <QtCore/qreadwritelock.h>
#include <QScrollBar>

class hal_filter_tab_bar;

class hal_logger_marshall;

struct hal_filter_item;

class channel_selector;

class hal_logger_widget : public content_widget
{
    Q_OBJECT

public:
    hal_logger_widget(QWidget* parent = 0);
    ~hal_logger_widget();

    virtual void setup_toolbar(toolbar* toolbar) Q_DECL_OVERRIDE;

    QPlainTextEdit* get_plain_text_edit();

    hal_filter_tab_bar* get_tab_bar();

    void reload_log_content();

    void resizeEvent(QResizeEvent* event) override;

Q_SIGNALS:
    void queue_log_update(spdlog::level::level_enum t, QString const& msg, hal_filter_item* filter);

public Q_SLOTS:

    //void handle_current_channel_changed(hal_channel_item* item);
    void handle_current_channel_updated(spdlog::level::level_enum t, QString const& msg);
    void handle_channel_updated(spdlog::level::level_enum t, const std::string& logger_name, std::string const& msg);
    void handle_current_channel_changed(int index);
    void show_text_edit_context_menu(const QPoint& point);
    void filter_item_clicked(const int& index);
    void handle_first_user_interaction(int value);
    //    void queue_log_update_adapter(spdlog::level::level_enum t, QString const &msg);

private:
    hal_filter_tab_bar* m_tab_bar;

    QPlainTextEdit* m_plain_text_edit;

    channel_selector* m_selector;

    hal_logger_marshall* m_log_marshall;

    std::string m_current_channel;

    QReadWriteLock m_lock;

    QScrollBar* m_plain_text_edit_scrollbar;

    bool m_user_interacted_with_scrollbar;

    void scroll_to_bottom();
};

#endif    // HAL_LOGGER_WIDGET_H
