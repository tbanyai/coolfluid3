// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_SFDM_CellTerm_hpp
#define CF_SFDM_CellTerm_hpp

#include "Solver/Action.hpp"

#include "SFDM/LibSFDM.hpp"

namespace CF {

namespace Mesh   { class Field; class FieldGroup; class CCells; }
namespace SFDM {

/////////////////////////////////////////////////////////////////////////////////////

class SFDM_API CellTerm : public CF::Solver::Action {

public: // typedefs

  /// provider
  typedef boost::shared_ptr< CellTerm > Ptr;
  typedef boost::shared_ptr< CellTerm const > ConstPtr;

public: // functions

  /// Contructor
  /// @param name of the component
  CellTerm ( const std::string& name );

  /// Virtual destructor
  virtual ~CellTerm();

  /// Get the class name
  static std::string type_name () { return "CellTerm"; }

  /// @name ACCESSORS
  //@{

  Mesh::FieldGroup& field_group() { return *m_field_group.lock(); }

  Mesh::Field& solution()    { return *m_solution.lock(); }

  Mesh::Field& residual()    { return *m_residual.lock(); }

  Mesh::Field& wave_speed()  { return *m_wave_speed.lock(); }

  //@} END ACCESSORS

protected: // function

  void link_fields();

protected: // data

  boost::weak_ptr<Mesh::FieldGroup> m_field_group;

  boost::weak_ptr<Mesh::Field> m_solution;     ///< access to the solution field

  boost::weak_ptr<Mesh::Field> m_residual;     ///< access to the residual field

  boost::weak_ptr<Mesh::Field> m_wave_speed;   ///< access to the wave_speed field

};

/////////////////////////////////////////////////////////////////////////////////////

} // SFDM
} // CF

#endif // CF_SFDM_CellTerm_hpp