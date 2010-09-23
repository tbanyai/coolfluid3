// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_GUI_Client_StatusPanel_hpp
#define CF_GUI_Client_StatusPanel_hpp

/////////////////////////////////////////////////////////////////////////////

#include <QTreeView>

#include "Common/CF.hpp"

#include "GUI/Client/UI/LibClientUI.hpp"

/////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace GUI {

namespace ClientCore { class StatusModel; }

namespace ClientUI {

/////////////////////////////////////////////////////////////////////////////

  class ClientUI_API StatusPanel : public QTreeView
  {
    Q_OBJECT

  public:

    StatusPanel(ClientCore::StatusModel * model, QWidget * parent = CFNULL);

    ~StatusPanel();

  private slots:

    void subSystemAdded(const QModelIndex & index);

  private:

    ClientCore::StatusModel * m_model;

  }; // class StatusPanel

/////////////////////////////////////////////////////////////////////////////

} // namespace ClientUI
} // namespace GUI
} // namespace CF

/////////////////////////////////////////////////////////////////////////////

#endif // CF_GUI_Client_StatusPanel_h
