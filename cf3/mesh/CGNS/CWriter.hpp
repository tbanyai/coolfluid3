// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_mesh_CGNS_CWriter_hpp
#define cf3_mesh_CGNS_CWriter_hpp

////////////////////////////////////////////////////////////////////////////////

#include "mesh/MeshWriter.hpp"

#include "mesh/CGNS/LibCGNS.hpp"
#include "mesh/CGNS/Shared.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace mesh {

  class CRegion;
  class CElements;

namespace CGNS {

//////////////////////////////////////////////////////////////////////////////

/// This class defines CGNS mesh format writer
/// @author Willem Deconinck
class Mesh_CGNS_API CWriter : public MeshWriter, public Shared
{

public: // typedefs

  typedef boost::shared_ptr<CWriter> Ptr;
  typedef boost::shared_ptr<CWriter const> ConstPtr;

private : // typedefs

  typedef std::vector<boost::shared_ptr<CElements const> > GroupedElements;
  typedef std::map<std::string, GroupedElements > GroupsMapType;

public: // functions

  /// constructor
  CWriter( const std::string& name );

  /// Gets the Class name
  static std::string type_name() { return "CWriter"; }

  virtual void write_from_to(const Mesh& mesh, const common::URI& path);

  virtual std::string get_format() { return "CGNS"; }

  virtual std::vector<std::string> get_extensions();

private: // functions

  void write_base();

  void write_zone(const CRegion& region);

  void write_section(const GroupedElements& grouped_elements);

//  void write_boco(const GroupedElements& grouped_elements);

private: // data

  std::string m_fileBasename;

  std::map<const CTable<Real>*, Uint> m_global_start_idx;

}; // end CWriter


////////////////////////////////////////////////////////////////////////////////

} // CGNS
} // mesh
} // cf3

////////////////////////////////////////////////////////////////////////////////

#endif // cf3_mesh_CGNS_CWriter_hpp