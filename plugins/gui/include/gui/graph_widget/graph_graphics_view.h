//  MIT License
//
//  Copyright (c) 2019 Ruhr University Bochum, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2021 Max Planck Institute for Security and Privacy. All Rights reserved.
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
#include "gui/gui_globals.h"
#include "gui/graph_widget/items/nodes/gates/graphics_gate.h"
#include "gui/graph_widget/graphics_scene.h"

#include <QGraphicsView>
#include <QAction>

namespace hal
{
    class GraphicsItem;
    class GraphWidget;

    namespace graph_widget_constants
    {
    enum class grid_type;
    }

    /**
     * @ingroup graph-visuals
     * @brief A view to display the rendered graph (needs a GraphicsScene).
     *
     * The GraphGraphicsView is the GraphicsView for a GraphicsScene. (Its like the camera that shows a section of
     * the scene)
     */
    class GraphGraphicsView : public QGraphicsView
    {
        Q_OBJECT

    public:
        /**
         * Constructor.
         *
         * @param parent - The parent GraphWidget
         */
        GraphGraphicsView(GraphWidget* parent);

        /**
         * Zoom in/out at the position of the mouse.
         *
         * @param factor - The new zoom factor
         */
        void gentleZoom(const qreal factor);

        /**
         * Zoom in/out at the center of the viewport.
         *
         * @param factor - The new zoom factor
         */
        void viewportCenterZoom(const qreal factor);

        GraphicsScene::GridType gridType();
        void setGridType(GraphicsScene::GridType gridType);

        Qt::KeyboardModifier dragModifier();
        void setDragModifier(Qt::KeyboardModifier dragModifier);

        Qt::KeyboardModifier panModifier();
        void setPanModifier(Qt::KeyboardModifier panModifier);

    Q_SIGNALS:
        /**
         * Q_SIGNAL that is emitted whenever the user double-clicks a module in the current view. <br>
         * Note that this signal is not emitted for double-clicks on gates or nets.
         *
         * @param id - The id of the module that was double clicked.
         */
        void moduleDoubleClicked(u32 id);

    private Q_SLOTS:
        void conditionalUpdate();
        void handleIsolationViewAction();
        void handleRemoveFromView();
        void handleMoveAction(u32 moduleId);
        void handleMoveNewAction();
        void handleRenameAction();
        void handleChangeTypeAction();
        void adjustMinScale();

        void handleFoldSingleAction();
        void handleFoldAllAction();
        void handleUnfoldSingleAction();
        void handleUnfoldAllAction();

        void handleSelectOutputs();
        void handleSelectInputs();
        void handleGroupingUnassign();
        void handleGroupingAssignNew();
        void handleGroupingAssingExisting();

        void handleModuleDialog();
        void handleCancelPickModule();
        void handleShortestPath();

    private:
        void paintEvent(QPaintEvent* event) override;
        void drawForeground(QPainter* painter, const QRectF& rect) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent* event) override;
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dragLeaveEvent(QDragLeaveEvent *event) override;
        void dragMoveEvent(QDragMoveEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void wheelEvent(QWheelEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

        void showContextMenu(const QPoint& pos);

        void updateMatrix(const int delta);

        void toggleAntialiasing();

        bool itemDraggable(GraphicsItem* item);

        struct LayouterPoint
        {
            int mIndex;
            qreal mPos;
        };
        QVector<QPoint> closestLayouterPos(const QPointF& scene_pos) const;
        LayouterPoint closestLayouterPoint(qreal scene_pos, int default_spacing, int min_index, QVector<qreal> sections) const;

        #ifdef GUI_DEBUG_GRID
        void debugShowLayouterGridpos(const QPoint& mouse_pos);
        void debugDrawLayouterGridpos(QPainter* painter);
        QPoint m_debug_gridpos = QPoint(0,0);
        bool mDebugGridposEnable = true;
        #endif

        GraphWidget* mGraphWidget;

        GraphicsItem* mItem;

        bool mMinimapEnabled;

        bool mGridEnabled;
        bool mGridClustersEnabled;
        GraphicsScene::GridType mGridType;

        QPoint mDragMousedownPosition;
        QPoint mDragStartGridpos;
        GraphicsGate* mDragItem;
        QPoint mDragCurrentGridpos;
        bool mDragCurrentModifier;
        bool mDropAllowed;

        Qt::KeyboardModifier mDragModifier;

        QPoint mMovePosition;
        Qt::KeyboardModifier mPanModifier;

        Qt::KeyboardModifier mZoomModifier;
        qreal mZoomFactorBase;
        QPointF mTargetScenePos;
        QPointF mTargetViewportPos;

        qreal mMinScale;

        static const QString sAssignToGrouping;
    };
}
