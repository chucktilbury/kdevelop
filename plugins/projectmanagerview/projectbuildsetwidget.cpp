/***************************************************************************
 *   This file is part of KDevelop                                         *
 *   Copyright 2007 Andreas Pakulat <apaku@gmx.de>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "projectbuildsetwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QToolButton>
#include <QStringListModel>
#include <QModelIndex>
#include <QStandardItemModel>

#include <kdebug.h>
#include <kicon.h>

#include "projectmodel.h"
#include "iproject.h"

#include "projectmanagerview_part.h"
#include "projectmanagerview.h"
#include "projectbuildsetmodel.h"

#include "ui_projectbuildsetwidget.h"

ProjectBuildSetWidget::ProjectBuildSetWidget( ProjectManagerView* view,
                                              ProjectManagerViewPart* plugin,
                                              QWidget* parent )
    : QWidget( parent ), m_view(view), m_plugin(plugin),
     m_ui( new Ui::ProjectBuildSetWidget )
{
    m_ui->setupUi( this );
    m_ui->addItemButton->setToolButtonStyle( Qt::ToolButtonIconOnly );
    m_ui->addItemButton->setIcon( KIcon( "list-add" ) );

    connect( m_ui->addItemButton, SIGNAL( clicked() ),
             this, SLOT( addItems() ) );

    m_ui->removeItemButton->setToolButtonStyle( Qt::ToolButtonIconOnly );
    m_ui->removeItemButton->setIcon( KIcon( "list-remove" ) );

    connect( m_ui->removeItemButton, SIGNAL( clicked() ),
             this, SLOT( removeItems() ) );

    kDebug(9511) << "setting model to:" << m_plugin->buildSet() << m_plugin;
    m_ui->itemView->setModel( m_plugin->buildSet() );

}

ProjectBuildSetWidget::~ProjectBuildSetWidget()
{
    delete m_ui;
}

void ProjectBuildSetWidget::addItems()
{
    foreach( KDevelop::ProjectBaseItem* item, m_view->selectedItems() )
    {
        m_plugin->buildSet()->addProjectItem( item );
    }
}

void ProjectBuildSetWidget::removeItems()
{
    foreach( QModelIndex idx, m_ui->itemView->selectionModel()->selectedIndexes() )
    {
        KDevelop::ProjectBaseItem* item = m_plugin->buildSet()->itemForIndex( idx );
        if( item )
        {
            m_plugin->buildSet()->removeProjectItem( item );
        }
    }
}

//kate: space-indent on; indent-width 4; replace-tabs on; auto-insert-doxygen on; indent-mode cstyle;
