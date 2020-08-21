#include "gui/selection_details_widget/gate_details_widget.h"
#include "gui/selection_details_widget/data_fields_table.h"
#include "gui/selection_details_widget/disputed_big_icon.h"
#include "gui/selection_details_widget/details_section_widget.h"

#include "netlist/gate.h"
#include "netlist/net.h"

#include "gui/gui_globals.h"
#include "gui/gui_utils/geometry.h"

#include "gui/graph_widget/graph_navigation_widget.h"
#include "netlist/module.h"

#include <QApplication>
#include <QCursor>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QResizeEvent>
#include <QScrollArea>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollBar>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QApplication> //to extract the stylesheet of the main app.
#include <QMenu>
#include <QIcon>
#include <QClipboard>

namespace hal
{
    GateDetailsWidget::GateDetailsWidget(QWidget* parent) : DetailsWidget(DetailsWidget::GateDetails, parent)
    {

        //this line throws a warning that there is already an existing layout, yet there is no layout set and
        //even after calling delete layout(); and then setting the layout, the warning continues
        m_scroll_area = new QScrollArea(this);
        m_top_lvl_container = new QWidget(m_scroll_area);
        m_top_lvl_layout = new QVBoxLayout(m_top_lvl_container);
        m_top_lvl_container->setLayout(m_top_lvl_layout);;
		m_content_layout = new QVBoxLayout(this);
        m_scroll_area->setWidget(m_top_lvl_container);
        m_scroll_area->setWidgetResizable(true);

        //layout customization
        m_content_layout->setContentsMargins(0,0,0,0);
        m_content_layout->setSpacing(0);
        m_top_lvl_layout->setContentsMargins(0,0,0,0);
        m_top_lvl_layout->setSpacing(0);

        //container-layouts to add spacing Widgets (gt = general table, op = output pins, etc)
        QHBoxLayout *intermediate_layout_gt = new QHBoxLayout();
        intermediate_layout_gt->setContentsMargins(3,3,0,0);
        intermediate_layout_gt->setSpacing(0);

        // buttons
        m_general_info_button = new QPushButton("Gate Information", this);
        m_general_info_button->setEnabled(false);

        // table initializations (section 1-4)
        m_general_table     = new QTableWidget(4,2,this);
        m_input_pins_table  = new QTableWidget(0,3, this);
        m_output_pins_table = new QTableWidget(0,3, this);
        m_dataFieldsTable   = new DataFieldsTable(this);

        // sections
        m_inputPinsSection  = new DetailsSectionWidget("Input Pins (%1)", m_input_pins_table, this);
        m_outputPinsSection = new DetailsSectionWidget("Output Pins (%1)", m_output_pins_table, this);
        m_dataFieldsSection = new DetailsSectionWidget("Data Fields (%1)", m_dataFieldsTable, this);

        //shared stlye options (every option is applied to each table)
        DetailsSectionWidget::setDefaultTableStyle(m_general_table);

        //customize general section by adding the fixed iitems
        QList<QTableWidgetItem*> tmp_general_info_list = {new QTableWidgetItem("Name:"), new QTableWidgetItem("Type:"),
                                                          new QTableWidgetItem("ID:"), new QTableWidgetItem("Module:")};
        for(int i = 0; i < tmp_general_info_list.size(); i++)
        {
            auto item = tmp_general_info_list.at(i);
            item->setFlags((Qt::ItemFlag)~Qt::ItemIsEnabled);
            item->setFont(m_keyFont);
            m_general_table->setItem(i, 0, item);
        }

        //create dynamic items that change when gate is changed
        m_name_item = new QTableWidgetItem();
        m_name_item->setFlags(Qt::ItemIsEnabled);
        m_general_table->setItem(0, 1, m_name_item);

        m_type_item = new QTableWidgetItem();
        m_type_item->setFlags(Qt::ItemIsEnabled);
        m_general_table->setItem(1, 1, m_type_item);

        m_id_item = new QTableWidgetItem();
        m_id_item->setFlags(Qt::ItemIsEnabled);
        m_general_table->setItem(2, 1, m_id_item);

        m_ModuleItem = new QTableWidgetItem();
        m_ModuleItem->setFlags(Qt::ItemIsEnabled);
        m_general_table->setItem(3, 1, m_ModuleItem);


        //(5) Boolean Function section
        m_boolean_functions_container = new QWidget(this);
        m_boolean_functions_container_layout = new QVBoxLayout(m_boolean_functions_container);
        m_boolean_functions_container_layout->setContentsMargins(6,5,0,0);
        m_boolean_functions_container_layout->setSpacing(0);
        m_boolean_functions_container->setLayout(m_boolean_functions_container_layout);
        m_booleanFunctionsSection = new DetailsSectionWidget(m_boolean_functions_container, "Boolean Functions (%1)", this);

        // place gate icon
        QLabel* img = new DisputedBigIcon("sel_gate", this);

        //adding things to intermediate layout (the one thats neccessary for the left spacing)
        intermediate_layout_gt->addWidget(m_general_table);
        intermediate_layout_gt->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Fixed));
        intermediate_layout_gt->addWidget(img);
        intermediate_layout_gt->setAlignment(img,Qt::AlignTop);

        //adding things to the main layout
        m_top_lvl_layout->addWidget(m_general_info_button);
        m_top_lvl_layout->addLayout(intermediate_layout_gt);
        m_top_lvl_layout->addSpacerItem(new QSpacerItem(0, 7, QSizePolicy::Expanding, QSizePolicy::Fixed));
        m_top_lvl_layout->addWidget(m_inputPinsSection);
        m_top_lvl_layout->addWidget(m_outputPinsSection);
        m_top_lvl_layout->addWidget(m_dataFieldsSection);
        m_top_lvl_layout->addWidget(m_booleanFunctionsSection);

        //necessary to add at the end
        m_top_lvl_layout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        m_content_layout->addWidget(m_scroll_area);

        //setup the navigation_table ("activated" by clicking on an input / output pin in the 2 tables)
        //delete the table manually so its not necessarry to add a property for the stylesheet(otherwise this table is styled like the others)
        m_navigation_table = new GraphNavigationWidget();
        m_navigation_table->setWindowFlags(Qt::CustomizeWindowHint);
        m_navigation_table->hide_when_focus_lost(true);
        m_navigation_table->hide();
        connect(m_navigation_table, &GraphNavigationWidget::navigation_requested, this, &GateDetailsWidget::handle_navigation_jump_requested);

        connect(m_input_pins_table, &QTableWidget::itemDoubleClicked, this, &GateDetailsWidget::handle_input_pin_item_clicked);
        connect(m_output_pins_table, &QTableWidget::itemDoubleClicked, this, &GateDetailsWidget::handle_output_pin_item_clicked);
        connect(m_general_table, &QTableWidget::itemDoubleClicked, this, &GateDetailsWidget::handle_general_table_item_clicked);

        //context menu connects
        connect(m_general_table, &QTableWidget::customContextMenuRequested, this, &GateDetailsWidget::handle_general_table_menu_requested);
        connect(m_input_pins_table, &QTableWidget::customContextMenuRequested, this, &GateDetailsWidget::handle_input_pin_table_menu_requested);
        connect(m_output_pins_table, &QTableWidget::customContextMenuRequested, this, &GateDetailsWidget::handle_output_pin_table_menu_requested);

        //install eventfilers
        m_general_table->viewport()->setMouseTracking(true);
        m_general_table->viewport()->installEventFilter(this);
        m_input_pins_table->viewport()->setMouseTracking(true);
        m_input_pins_table->viewport()->installEventFilter(this);
        m_output_pins_table->viewport()->setMouseTracking(true);
        m_output_pins_table->viewport()->installEventFilter(this);

        //extract the width of the scrollbar out of the stylesheet to fix a scrollbar related bug
        QString main_stylesheet = qApp->styleSheet();
        main_stylesheet.replace("\n", ""); //remove newlines so the regex is a bit easier
        QRegularExpression re(".+?QScrollBar:vertical ?{[^}]+?(?: *width *?|; *width *?): *([0-9]*)[^;]*");
        QRegularExpressionMatch ma = re.match(main_stylesheet);
        m_scrollbar_width = (ma.hasMatch()) ? ma.captured(1).toInt() : 0;
    }

    GateDetailsWidget::~GateDetailsWidget()
    {
        delete m_navigation_table;
    }

    void GateDetailsWidget::handle_gate_name_changed(Gate* gate)
    {
        if (m_currentId == gate->get_id())
            update(m_currentId);
    }

    void GateDetailsWidget::handle_gate_removed(Gate* gate)
    {
        if (m_currentId == gate->get_id())
        {
            m_general_table->setHidden(true);
            m_scroll_area->setHidden(true);
        }
    }

    void GateDetailsWidget::handle_net_name_changed(Net* net)
    {
        bool update_needed = false;

        //check if currently shown gate is a src of renamed net
        for (auto& e : net->get_sources())
        {
            if (m_currentId == e.get_gate()->get_id())
            {
                update_needed = true;
                break;
            }
        }

        //check if currently shown gate is a dst of renamed net
        if (!update_needed)
        {
            for (auto& e : net->get_destinations())
            {
                if (m_currentId == e.get_gate()->get_id())
                {
                    update_needed = true;
                    break;
                }
            }
        }

        if (update_needed)
            update(m_currentId);
    }

    void GateDetailsWidget::handle_net_source_added(Net* net, const u32 src_gate_id)
    {
        Q_UNUSED(net);
        if (m_currentId == src_gate_id)
            update(m_currentId);
    }

    void GateDetailsWidget::handle_net_source_removed(Net* net, const u32 src_gate_id)
    {
        Q_UNUSED(net);
        if (m_currentId == src_gate_id)
            update(m_currentId);
    }

    void GateDetailsWidget::handle_net_destination_added(Net* net, const u32 dst_gate_id)
    {
        Q_UNUSED(net);
        if (m_currentId == dst_gate_id)
            update(m_currentId);
    }

    void GateDetailsWidget::handle_net_destination_removed(Net* net, const u32 dst_gate_id)
    {
        Q_UNUSED(net);
        if (m_currentId == dst_gate_id)
            update(m_currentId);
    }


    void GateDetailsWidget::handle_input_pin_item_clicked(const QTableWidgetItem *item)
    {
        if(item->column() != 2)
            return;

        int net_id = item->data(Qt::UserRole).toInt();

        auto clicked_net = g_netlist->get_net_by_id(net_id);

        if(!clicked_net)
            return;

        auto sources = clicked_net->get_sources();

        if(sources.empty() || clicked_net->is_global_input_net())
        {
            g_selection_relay.clear();
            g_selection_relay.m_selected_nets.insert(net_id);
            g_selection_relay.relay_selection_changed(this);
        }
        else if(sources.size() == 1)
        {
            auto ep = *sources.begin();
            g_selection_relay.clear();
            g_selection_relay.m_selected_gates.insert(ep.get_gate()->get_id());
            g_selection_relay.m_focus_type = SelectionRelay::item_type::gate;
            g_selection_relay.m_focus_id   = ep.get_gate()->get_id();
            g_selection_relay.m_subfocus   = SelectionRelay::subfocus::right;

            auto pins                          = ep.get_gate()->get_output_pins();
            auto index                         = std::distance(pins.begin(), std::find(pins.begin(), pins.end(), ep.get_pin()));
            g_selection_relay.m_subfocus_index = index;

            update(ep.get_gate()->get_id());
            g_selection_relay.relay_selection_changed(this);
        }
        else
        {
            m_navigation_table->setup(hal::node{hal::node_type::gate, 0}, clicked_net, false);
            m_navigation_table->move(QCursor::pos());
            m_navigation_table->show();
            m_navigation_table->setFocus();
        }
    }

    void GateDetailsWidget::handle_output_pin_item_clicked(const QTableWidgetItem *item)
    {
        if(item->column() != 2)
            return;

        int net_id = item->data(Qt::UserRole).toInt();
        Net* clicked_net = g_netlist->get_net_by_id(net_id);

        if(!clicked_net)
            return;

        auto destinations = clicked_net->get_destinations();
        if(destinations.empty() || clicked_net->is_global_output_net())
        {
            g_selection_relay.clear();
            g_selection_relay.m_selected_nets.insert(net_id);
            g_selection_relay.relay_selection_changed(this);
        }
        else if (destinations.size() == 1)
        {
            auto ep = *destinations.begin();
            g_selection_relay.clear();
            g_selection_relay.m_selected_gates.insert(ep.get_gate()->get_id());
            g_selection_relay.m_focus_type = SelectionRelay::item_type::gate;
            g_selection_relay.m_focus_id   = ep.get_gate()->get_id();
            g_selection_relay.m_subfocus   = SelectionRelay::subfocus::left;

            auto pins                          = ep.get_gate()->get_input_pins();
            auto index                         = std::distance(pins.begin(), std::find(pins.begin(), pins.end(), ep.get_pin()));
            g_selection_relay.m_subfocus_index = index;

            update(ep.get_gate()->get_id());
            g_selection_relay.relay_selection_changed(this);
        }
        else
        {
            m_navigation_table->setup(hal::node{hal::node_type::gate, 0}, clicked_net, true);
            m_navigation_table->move(QCursor::pos());
            m_navigation_table->show();
            m_navigation_table->setFocus();
        }
    }

    void GateDetailsWidget::handle_input_pin_table_menu_requested(const QPoint &pos)
    {
        if(!m_input_pins_table->itemAt(pos) || m_input_pins_table->itemAt(pos)->column() != 2)
            return;

        QMenu menu;

        auto clicked_net = g_netlist->get_net_by_id(m_input_pins_table->itemAt(pos)->data(Qt::UserRole).toInt());
        if(!g_netlist->is_global_input_net(clicked_net))
        {
            menu.addAction("Jump to source gate", [this, pos](){
                handle_input_pin_item_clicked(m_input_pins_table->itemAt(pos));
            });
        }

        menu.addAction(QIcon(":/icons/python"), "Extract net as python code (copy to clipboard)",[this, pos](){
            QApplication::clipboard()->setText("netlist.get_net_by_id(" + m_input_pins_table->itemAt(pos)->data(Qt::UserRole).toString() + ")");
        });

        menu.addAction(QIcon(":/icons/python"), "Extract sources as python code (copy to clipboard)",[this, pos](){
            QApplication::clipboard()->setText("netlist.get_net_by_id(" + m_input_pins_table->itemAt(pos)->data(Qt::UserRole).toString() + ").get_sources()" );
        });

        menu.move(dynamic_cast<QWidget*>(sender())->mapToGlobal(pos));
        menu.exec();

    }

    void GateDetailsWidget::handle_output_pin_table_menu_requested(const QPoint &pos)
    {
        if(!m_output_pins_table->itemAt(pos) || m_output_pins_table->itemAt(pos)->column() != 2)
            return;

        QMenu menu;

        auto clicked_net = g_netlist->get_net_by_id(m_output_pins_table->itemAt(pos)->data(Qt::UserRole).toInt());
        if(!g_netlist->is_global_output_net(clicked_net))
        {
            menu.addAction("Jump to destination gate", [this, pos](){
                handle_output_pin_item_clicked(m_output_pins_table->itemAt(pos));
            });
        }
        menu.addAction(QIcon(":/icons/python"), "Extract net as python code (copy to clipboard)",[this, pos](){
            QApplication::clipboard()->setText("netlist.get_net_by_id(" + m_output_pins_table->itemAt(pos)->data(Qt::UserRole).toString() + ")");
        });

        menu.addAction(QIcon(":/icons/python"), "Extract destinations as python code (copy to clipboard)",[this, pos](){
            QApplication::clipboard()->setText("netlist.get_net_by_id(" + m_output_pins_table->itemAt(pos)->data(Qt::UserRole).toString() + ").get_destinations()" );
        });

        menu.move(dynamic_cast<QWidget*>(sender())->mapToGlobal(pos));
        menu.exec();
    }

    void GateDetailsWidget::handle_general_table_menu_requested(const QPoint &pos)
    {
        if(!m_general_table->itemAt(pos) || m_general_table->itemAt(pos)->column() != 1)
            return;

        QMenu menu;
        QString description;
        QString python_command = "netlist.get_gate_by_id(" + QString::number(m_currentId) + ").";
        QString raw_string = "", raw_desc = "";
        switch(m_general_table->itemAt(pos)->row())
        {
            case 0: python_command += "get_name()"; description = "Extract name as python code (copy to clipboard)";
                    raw_string = m_general_table->itemAt(pos)->text(); raw_desc = "Extract raw name (copy to clipboard)"; break;
            case 1: python_command += "get_type()"; description = "Extract type as python code (copy to clipboard)";
                    raw_string = m_general_table->itemAt(pos)->text(); raw_desc = "Extraxt raw type(copy to clipboard)";break;
            case 2: python_command += "get_id()"; description = "Extract id as python code (copy to clipboard)";
                    raw_string = m_general_table->itemAt(pos)->text(); raw_desc = "Extract raw id as string (copy to clipboard)";break;
            case 3: python_command += "get_module()"; description = "Extract module as python code (copy to clipboard)"; break;
        }

        if(!raw_desc.isEmpty())
        {
            menu.addAction(raw_desc,[raw_string](){
                QApplication::clipboard()->setText(raw_string);
            });
        }
        menu.addAction(QIcon(":/icons/python"), description, [python_command](){
            QApplication::clipboard()->setText(python_command);
        });

        menu.move(dynamic_cast<QWidget*>(sender())->mapToGlobal(pos));
        menu.exec();
    }

    QSize GateDetailsWidget::calculate_table_size(QTableWidget *table)
    {
        //necessary to test if the table is empty, otherwise (due to the resizeColumnsToContents function)
        //is the tables width far too big, so just return 0 as the size
        if(!table->rowCount())
            return QSize(0,0);

        int w = table->verticalHeader()->width() + 4; // +4 seems to be needed
        for (int i = 0; i < table->columnCount(); i++)
           w += table->columnWidth(i); // seems to include gridline
        int h = table->horizontalHeader()->height() + 4;
        for (int i = 0; i < table->rowCount(); i++)
           h += table->rowHeight(i);
        return QSize(w+5, h);

    }


    void GateDetailsWidget::handle_module_removed(Module* module)
    {
        if (m_currentId == 0)
            return;
        auto g = g_netlist->get_gate_by_id(m_currentId);

        if (module->contains_gate(g))
        {
            update(m_currentId);
        }
    }

    void GateDetailsWidget::handle_module_name_changed(Module* module)
    {
        if (m_currentId == 0)
            return;
        auto g = g_netlist->get_gate_by_id(m_currentId);

        if (module->contains_gate(g))
        {
            update(m_currentId);
        }
    }

    void GateDetailsWidget::handle_module_gate_assigned(Module* module, u32 associated_data)
    {
        Q_UNUSED(module);
        if (m_currentId == associated_data)
        {
            update(m_currentId);
        }
    }

    void GateDetailsWidget::handle_module_gate_removed(Module* module, u32 associated_data)
    {
        Q_UNUSED(module);
        if (!g_netlist->is_gate_in_netlist(g_netlist->get_gate_by_id(associated_data)))
            return;

        if (m_currentId == associated_data)
        {
            update(m_currentId);
        }
    }

    void GateDetailsWidget::resizeEvent(QResizeEvent* event)
    {
        //2 is needed because just the scrollbarwitdth is not enough (does not include its border?)
        m_boolean_functions_container->setFixedWidth(event->size().width() - m_scrollbar_width-2);
    }

    bool GateDetailsWidget::eventFilter(QObject *watched, QEvent *event)
    {
        if((watched == m_input_pins_table->viewport() || watched == m_output_pins_table->viewport()) && event->type() == QEvent::MouseMove)
        {
            //need to determine which of the tables is the "owner" of the viewport
            QTableWidget* table = (watched == m_input_pins_table->viewport()) ? m_input_pins_table : m_output_pins_table;
            QMouseEvent* ev = dynamic_cast<QMouseEvent*>(event);
            QTableWidgetItem* item = table->itemAt(ev->pos());
            if(item)
            {
                if(item->column() == 2)
                    setCursor(QCursor(Qt::PointingHandCursor));
                else
                    setCursor(QCursor(Qt::ArrowCursor));
            }
            else
                setCursor(QCursor(Qt::ArrowCursor));

        }
        if(watched == m_general_table->viewport() && event->type() == QEvent::MouseMove)
        {
            QTableWidgetItem* item = m_general_table->itemAt(dynamic_cast<QMouseEvent*>(event)->pos());
            if(item == m_ModuleItem)
                setCursor(QCursor(Qt::PointingHandCursor));
            else
                setCursor(QCursor(Qt::ArrowCursor));
        }

        //restore default cursor when leaving any watched widget (maybe save cursor before entering?)
        if(event->type() == QEvent::Leave)
            setCursor(QCursor(Qt::ArrowCursor));

        return false;
    }

    void GateDetailsWidget::update(const u32 gate_id)
    {
        auto g = g_netlist->get_gate_by_id(gate_id);
        m_currentId = gate_id;

        if(!g || m_currentId == 0)
            return;

        //update (1)general info section
        m_name_item->setText(QString::fromStdString(g->get_name()));
        m_type_item->setText(QString::fromStdString(g->get_type()->get_name()));
        m_id_item->setText(QString::number(m_currentId));

        QString module_text = "";
        for (const auto sub : g_netlist->get_modules())
        {
            if (sub->contains_gate(g))
            {
                module_text = QString::fromStdString(sub->get_name()) + "[" + QString::number(sub->get_id()) + "]";
                m_ModuleItem->setData(Qt::UserRole, sub->get_id());
            }
        }
        m_ModuleItem->setText(module_text);
        m_general_table->resizeColumnsToContents();
        m_general_table->setFixedWidth(calculate_table_size(m_general_table).width());

        //update (2)input-pin section
        m_input_pins_table->clearContents();
        m_inputPinsSection->setRowCount(g->get_input_pins().size());
        m_input_pins_table->setRowCount(g->get_input_pins().size());
        m_input_pins_table->setMaximumHeight(m_input_pins_table->verticalHeader()->length());
        int index = 0;
        for(const auto &pin : g->get_input_pins())
        {
            QTableWidgetItem* pin_name = new QTableWidgetItem(QString::fromStdString(pin));
            QTableWidgetItem* arrow_item = new QTableWidgetItem(QChar(0x2b05));
            QTableWidgetItem* net_item = new QTableWidgetItem();
            arrow_item->setForeground(QBrush(QColor(114, 140, 0), Qt::SolidPattern));
            //pin_name->setFlags(Qt::ItemIsEnabled);
            pin_name->setFlags((Qt::ItemFlag)~Qt::ItemIsEnabled);
            arrow_item->setFlags((Qt::ItemFlag)~Qt::ItemIsEnabled);
            //arrow_item->setFlags((Qt::ItemFlag)(~Qt::ItemIsSelectable));
            net_item->setFlags(Qt::ItemIsEnabled);

            auto input_net = g_netlist->get_gate_by_id(gate_id)->get_fan_in_net(pin);
            if(input_net)
            {
                net_item->setText(QString::fromStdString(input_net->get_name()));
                net_item->setData(Qt::UserRole, input_net->get_id());
            }
            else
                net_item->setText("unconnected");

            m_input_pins_table->setItem(index, 0, pin_name);
            m_input_pins_table->setItem(index, 1, arrow_item);
            m_input_pins_table->setItem(index, 2, net_item);
            index++;
        }
        m_input_pins_table->resizeColumnsToContents();
        m_input_pins_table->setFixedWidth(calculate_table_size(m_input_pins_table).width());

        //update(3) output pins section
        m_output_pins_table->clearContents();
        m_outputPinsSection->setRowCount(g->get_output_pins().size());
        m_output_pins_table->setRowCount(g->get_output_pins().size());
        m_output_pins_table->setMaximumHeight(m_output_pins_table->verticalHeader()->length());
        index = 0;
        for(const auto &pin : g->get_output_pins())
        {
            QTableWidgetItem* pin_name = new QTableWidgetItem(QString::fromStdString(pin));
            QTableWidgetItem* arrow_item = new QTableWidgetItem(QChar(0x27a1));
            QTableWidgetItem* net_item = new QTableWidgetItem();
            arrow_item->setForeground(QBrush(QColor(114, 140, 0), Qt::SolidPattern));//stylesheet?
            //pin_name->setFlags(Qt::ItemIsEnabled);
            pin_name->setFlags((Qt::ItemFlag)~Qt::ItemIsEnabled);
            //arrow_item->setFlags(Qt::ItemIsEnabled);
            arrow_item->setFlags((Qt::ItemFlag)~Qt::ItemIsEnabled);
            net_item->setFlags(Qt::ItemIsEnabled);

            auto output_net = g_netlist->get_gate_by_id(gate_id)->get_fan_out_net(pin);
            if(output_net)
            {
                net_item->setText(QString::fromStdString(output_net->get_name()));
                net_item->setData(Qt::UserRole, output_net->get_id());
            }
            else
                net_item->setText("unconnected");

            m_output_pins_table->setItem(index, 0, pin_name);
            m_output_pins_table->setItem(index, 1, arrow_item);
            m_output_pins_table->setItem(index, 2, net_item);
            index++;
        }
        m_output_pins_table->resizeColumnsToContents();
        m_output_pins_table->setFixedWidth(calculate_table_size(m_output_pins_table).width());

        //update(4) data fields section
        m_dataFieldsSection->setRowCount(g->get_data().size());
        m_dataFieldsTable->updateData(gate_id,g->get_data());

        //update(5) boolean functions section
        //clear container layout
        while(m_boolean_functions_container_layout->itemAt(0) != 0)
        {
            QLayoutItem* i = m_boolean_functions_container_layout->takeAt(0);
            delete i->widget();
            delete i;
        }

        m_booleanFunctionsSection->setRowCount(g->get_boolean_functions().size());
        QFrame* last_line = nullptr; //unexpected behaviour below otherwise
        for(const auto& it : g->get_boolean_functions())
        {
            QLabel* fnct = new QLabel(QString::fromStdString(it.first) + " = " + QString::fromStdString(it.second.to_string()));
            fnct->setWordWrap(true);
            m_boolean_functions_container_layout->addWidget(fnct);
            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            //to outsource this line into the stylesheet you need to make a new class that inherits from QFrame
            //and style that class. properties and the normal way does not work (other tables are also affected)
            line->setStyleSheet("QFrame{background-color: gray;}");
            last_line = line;
            m_boolean_functions_container_layout->addWidget(line);
        }

        if(last_line){
            m_boolean_functions_container_layout->removeWidget(last_line);
            delete last_line;
        }

        //to prevent any updating(render) errors that can occur, manually tell the tables to update
        m_general_table->update();
        m_input_pins_table->update();
        m_output_pins_table->update();
        m_dataFieldsTable->update();
    }

    void GateDetailsWidget::handle_navigation_jump_requested(const hal::node origin, const u32 via_net, const QSet<u32>& to_gates)
    {
        Q_UNUSED(origin);

        auto n = g_netlist->get_net_by_id(via_net);

        if (to_gates.isEmpty() || !n)
            return;
        for (u32 id : to_gates)
        {
            if (!g_netlist->get_gate_by_id(id))
                return;
        }

        m_navigation_table->hide();
        g_selection_relay.clear();
        g_selection_relay.m_selected_gates = to_gates;
        if (to_gates.size() == 1)
        {
            g_selection_relay.m_focus_type = SelectionRelay::item_type::gate;
            auto g                         = g_netlist->get_gate_by_id(*to_gates.constBegin());
            g_selection_relay.m_focus_id   = g->get_id();
            g_selection_relay.m_subfocus   = SelectionRelay::subfocus::left;

            u32 index_cnt = 0;
            for (const auto& pin : g->get_input_pins())
            {
                if (g->get_fan_in_net(pin) == n)
                {
                    g_selection_relay.m_subfocus_index = index_cnt;
                    break;
                }
                index_cnt++;
            }

            g_selection_relay.relay_selection_changed(this);
        }
        m_navigation_table->hide();

        // TODO ensure gates visible in graph
    }

    void GateDetailsWidget::handle_general_table_item_clicked(const QTableWidgetItem *item)
    {
        //cant get the item from the index (static_cast<QTableWidgetItem*>(index.internalPointer()) fails),
        //so ask the item QTableWidgetItem directly
        if (item->row() == m_ModuleItem->row() && item->column() == m_ModuleItem->column())
        {
            g_selection_relay.clear();
            g_selection_relay.m_selected_modules.insert(m_ModuleItem->data(Qt::UserRole).toInt());
            g_selection_relay.relay_selection_changed(this);
        }
    }
}
