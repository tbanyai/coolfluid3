// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Common_LibCommon_hpp
#define CF_Common_LibCommon_hpp

////////////////////////////////////////////////////////////////////////////////

#include "Common/CF.hpp"
#include "Common/CommonAPI.hpp"
#include "Common/LibraryRegister.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Common {

/// Class defines the initialization and termination of the library Mesh
/// @author Tiago Quintino
class Common_API LibCommon : public Common::LibraryRegister<LibCommon>
{
public:

  /// Static function that returns the module name.
  /// Must be implemented for the LibraryRegister template
  /// @return name of the library
  static std::string library_name() { return "Common"; }

  /// Static function that returns the description of the module.
  /// Must be implemented for the LibraryRegister template
  /// @return description of the library

  static std::string library_description()
  {
    return "This library implements the Common API.";
  }

  /// Gets the Class name
  static std::string type_name() { return "LibCommon"; }

  /// @return string of the library namespace
  static std::string library_namespace() { return "CF.Common"; }

  /// initiate library
  virtual void initiate();

  /// terminate library
  virtual void terminate();

}; // LibCommon

} // Common
} // CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Common_LibCommon_hpp
