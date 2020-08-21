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

#include <QWidget>

namespace hal
{
    class NotificationManagerQssAdapter : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(int x_offset READ x_offset WRITE set_x_offset)
        Q_PROPERTY(int y_offset READ y_offset WRITE set_y_offset)
        Q_PROPERTY(int spacing READ spacing WRITE set_spacing)

    public:
        explicit NotificationManagerQssAdapter(QWidget* parent = nullptr);

        void repolish();

        int x_offset() const;
        int y_offset() const;
        int spacing() const;

        void set_x_offset(const int offset);
        void set_y_offset(const int offset);
        void set_spacing(const int spacing);

        // LEFT PUBLIC INTENTIONALLY
        int m_x_offset;
        int m_y_offset;
        int m_spacing;
    };
}
