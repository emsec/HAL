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

#include "gui/new_selection_details_widget/general_table_widget.h"

namespace hal
{
    class Module;

    class ModuleInfoTable : public GeneralTableWidget
    {
        Q_OBJECT

    public:

        ModuleInfoTable(QWidget* parent = nullptr);
        void update(u32 moduleId);

    private:

        Module* mModule;

        QString name() const;
        QString id() const;
        QString type() const;
        QString module() const;
        QString numberOfGates() const;
        QString numberOfSubModules() const;
        QString numberOfNets() const;

        void changeName();
        void copyName() const;
        void pyCopyName() const;

        void copyId() const;

        void changeType() const;
        void copyType() const;
        void pyCopyType() const;

        void copyModule() const;
        void pyCopyModule() const;

        void copyNumberOfGates() const;
        
        void copyNumberOfSubmodules() const;

        void copyNumberOfNets() const;

        void navModule();

        QMenu* mNameEntryContextMenu;
        QMenu* mIdEntryContextMenu;
        QMenu* mTypeEntryContextMenu;
        QMenu* mModuleEntryContextMenu;
        QMenu* mNumOfGatesContextMenu;
        QMenu* mNumOfSubmodulesContextMenu;
        QMenu* mNumOfNetsContextMenu;

        std::function<void()> mModuleDoubleClickedAction;
    }; 
}
