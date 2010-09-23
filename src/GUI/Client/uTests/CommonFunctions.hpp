// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_GUI_Client_uTests_CommonFunctions_hpp
#define CF_GUI_Client_uTests_CommonFunctions_hpp

////////////////////////////////////////////////////////////////////////////

#include "GUI/Client/Core/NRoot.hpp"

////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace GUI {
namespace ClientTest {

  //////////////////////////////////////////////////////////////////////////

  ClientCore::NRoot::Ptr makeTreeFromFile();

  class CommonFunctionsTest : public QObject
  {
    Q_OBJECT

  private slots:

    void test_makeTreeFromFile();

  }; // class CommonFunctionsTest

  //////////////////////////////////////////////////////////////////////////

} // namespace ClientTest
} // namespace GUI
} // namespace CF

////////////////////////////////////////////////////////////////////////////

#endif // CF_GUI_Client_uTests_CommonFunctions_hpp
