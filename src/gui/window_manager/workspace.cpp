#include "window_manager/workspace.h"

#include <QStyle>

workspace::workspace(QWidget* parent) : QFrame(parent)
{

}

void workspace::repolish()
{
    QStyle* s = style();

    s->unpolish(this);
    s->polish(this);
}
