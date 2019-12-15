/*
 * MIT License
 *
 * Copyright (c) 2019 Chair for Embedded Security, Ruhr-University Bochum
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MODULE_MODEL_H
#define MODULE_MODEL_H

#include "def.h"

#include "gui/gui_utils/sort.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class module_item;

class module_model : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit module_model(QObject* parent = nullptr);

    // PURE VIRTUAL
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    // VIRTUAL
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    module_item* get_item(const QModelIndex& index) const;
    QModelIndex get_index(const module_item* const item) const;

    void init();
    void clear();

    void add_module(const u32 id, const u32 parent_module);
    void remove_module(const u32 id);
    void update_module(const u32 id);

private Q_SLOTS:
    void handle_global_setting_changed(void* sender, const QString& key, const QVariant& value);

private:
    module_item* m_top_module_item;

    QMap<u32, module_item*> m_module_items;

    gui_utility::sort_mechanism m_sort_mechanism;
};

#endif // MODULE_MODEL_H
