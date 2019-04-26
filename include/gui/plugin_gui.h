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

#ifndef __HAL_PLUGIN_GUI_H__
#define __HAL_PLUGIN_GUI_H__

#include "core/interface_interactive_ui.h"

class plugin_gui : public i_interactive_ui
{
public:
    /** constructor (= default) */
    plugin_gui() = default;
    /** destructor (= default) */
    ~plugin_gui() = default;

    /** interface implementation: i_base */
    std::string get_name() override;
    /** interface implementation: i_base */
    std::string get_version() override;
    /** interface implementation: i_base */
    std::set<interface_type> get_type() override;

    /** interface implementation: i_base */
    void initialize_logging() override;

    /** interface implementation: i_interactive_ui */
    bool exec(program_arguments& args) override;
};

#endif /* __HAL_PLUGIN_GUI_H__ */
