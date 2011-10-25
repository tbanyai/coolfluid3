// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_solver_Actions_CComputeArea_hpp
#define cf3_solver_Actions_CComputeArea_hpp

#include "solver/Actions/CLoopOperation.hpp"

/////////////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace mesh { class CScalarFieldView; }
namespace solver {
namespace Actions {

///////////////////////////////////////////////////////////////////////////////////////

class solver_Actions_API CComputeArea : public CLoopOperation
{
public: // typedefs

  /// pointers
  typedef boost::shared_ptr<CComputeArea> Ptr;
  typedef boost::shared_ptr<CComputeArea const> ConstPtr;

public: // functions
  /// Contructor
  /// @param name of the component
  CComputeArea ( const std::string& name );

  /// Virtual destructor
  virtual ~CComputeArea() {}

  /// Get the class name
  static std::string type_name () { return "CComputeArea"; }

  /// execute the action
  virtual void execute ();

private: // helper functions

  void config_field();

  void trigger_elements();

private: // data

  boost::weak_ptr<mesh::Field> m_area;
  boost::weak_ptr<mesh::Space> m_area_field_space;

  RealMatrix m_coordinates;

};

/////////////////////////////////////////////////////////////////////////////////////

} // Actions
} // solver
} // cf3

/////////////////////////////////////////////////////////////////////////////////////

#endif // cf3_solver_Actions_CComputeArea_hpp