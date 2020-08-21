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

#include <QFrame>
#include <QPair>
#include <QVector>

#include <memory>

class QFormLayout;
class QLineEdit;
class QPushButton;
class QVBoxLayout;

namespace hal
{
    class CLIPluginInterface;
    class ProgramArguments;

    class PluginArgumentsWidget : public QFrame
    {
        Q_OBJECT

    public:
        explicit PluginArgumentsWidget(QWidget* parent = nullptr);

        ProgramArguments get_args();

        void setup_plugin_layout(const QString& plugin);

    public Q_SLOTS:
        void setup(const QString& plugin_name);
        void handle_plugin_selected(int index);

        void handle_button_clicked(bool checked);
        void handle_text_edited(const QString& text);

    private:
        char* to_heap_cstring(const QString& string);

        QFormLayout* m_form_layout;

        CLIPluginInterface* m_plugin;

        QVector<QPair<QPushButton*, QLineEdit*>> m_vector;

        int m_current_index;
    };
}
