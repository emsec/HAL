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

#include "core/log.h"

#include "log_channel_manager/log_channel_manager_constants.h"

#include <QString>

#include <vector>

namespace hal
{
    struct log_entry
    {
        QString m_message;
        spdlog::level::level_enum m_type;
    };

    struct log_channel
    {
        QString m_name;

        log_entry m_entries[LogChannelManager_constants::max_entries];

        int m_entry_count;
        int m_first_entry;

        int unread_entries;
        int unread_warnings;
        int unread_errors;
        int unread_successes;
    };

    class LogChannelManager
    {
    public:
        LogChannelManager();

        void logmanager_callback(const spdlog::details::log_msg& msg);

    private:
        std::vector<log_channel*> m_fixed_channels;
        log_channel* m_temporary_channels[LogChannelManager_constants::max_channels];
    };
}
