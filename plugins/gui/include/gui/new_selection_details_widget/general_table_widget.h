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

#include "hal_core/defines.h"

#include <QTableWidget>
#include <QMap>

namespace hal
{
    class GeneralTableWidget : public QTableWidget
    {
        Q_OBJECT

    public:

        /**
         * The constructor.
         *
         * @param parent - The widget's parent.
         */
        GeneralTableWidget(QWidget* parent = nullptr);

        /**
         * Updates the table with the data of the element with the given id and adjusts it's size.
         *
         * @param elementId - The id of the element.
         */
        virtual void setContent(u32 elementId);

    protected:
        /**
         * Appends a row to the table.
         *
         * @param key - The key displayed in the first column of the row.
         * @param val - The value displayed in the second column of the row.
         * @param contextMenu - The context menu that will be displayed when right clicking the row.
         */
        void setRow(const QString& key, const QString& val, QMenu* contextMenu = nullptr, std::function<void()> doubleClickAction = nullptr);

    private:
        /**
         * Displays the context menu at the given point.
         *
         * @param point - The displayed text of the header.
         */
        void serveContextMenu(const QPoint& point) const;

        /**
        * Handles double clicks on the table.
        *
        * @param index - The index that has been double clicked on.
        */
        void handleDoubleClick(const QModelIndex& index) const;

        /**
        * Updates the table with the data of the element with the given id.
        *
        * @param id - The id of the element.
        */
        virtual void update(u32 id) = 0;

        /**
        * Adjusts the size of the table so that all elements will fit.
        */
        void adjustSize();


        QMap<QString, QTableWidgetItem*> mKeyItemMap;
        QMap<int, QMenu*> mRowMenuMap;
        QMap<int, std::function<void()>> mRowDoubleClickActionMap;
    };
}
