// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Mesh_LagrangeP1_Line2D_hpp
#define CF_Mesh_LagrangeP1_Line2D_hpp

#include "Mesh/ElementType.hpp"
#include "Mesh/LagrangeP1/Line.hpp"

namespace CF {
namespace Mesh {
  template <typename SF> class ShapeFunctionT;
namespace LagrangeP1 {

////////////////////////////////////////////////////////////////////////////////

/// @brief Lagrange P1 Triangular Element type
/// This class provides the lagrangian shape function describing the
/// representation of the solution and/or the geometry in a P1 (linear)
/// triangular element.
/// @see ElementType for documentation of undocumented functions
/// @author Willem Deconinck
/// @author Tiago Quintino
/// @author Bart Janssens
struct Mesh_LagrangeP1_API Line2D
{
public: // typedefs

  typedef boost::shared_ptr<Line2D>       Ptr;
  typedef boost::shared_ptr<Line2D const> ConstPtr;

  /// The shape function of this element
  typedef Line SF;

  /// @name Element definitions
  //  -------------------------
  //@{
  enum { shape          = SF::shape          };
  enum { dimensionality = SF::dimensionality };
  enum { nb_nodes       = SF::nb_nodes       };
  enum { order          = SF::order          };

  enum { dimension      = 2 };
  enum { nb_faces       = 1 };
  enum { nb_edges       = 2 };
  //@}

  /// @name Matrix Types
  //  --------------------------------
  //@{
  typedef SF::MappedCoordsT                              MappedCoordsT;
  typedef Eigen::Matrix<Real, dimension, 1>              CoordsT;
  typedef Eigen::Matrix<Real, nb_nodes, dimension>       NodesT;
  typedef Eigen::Matrix<Real, dimensionality, dimension> JacobianT;
  //@}

public: // functions

  /// @name Constructor / Destructor / Type name
  //  ------------------------------------------
  //@{

  Line2D() {}
  ~Line2D() {}
  static std::string type_name() { return "Line2D"; }

  //@}

  /// @name Accessor functions
  //  ------------------------
  //@{

  static const ShapeFunctionT<SF>& shape_function();
  static const ElementType::FaceConnectivity& faces();
  static const ElementType& face_type(const Uint face);

  //@}

  /// @name Computation functions
  //  ---------------------------
  //@{

  static Real volume(const NodesT& nodes);
  static Real area(const NodesT& nodes);
  static void compute_centroid(const NodesT& nodes , CoordsT& centroid);
  static void compute_normal(const NodesT& nodes , CoordsT& normal);

  //@}

  /// @name Non-API static functions
  //  ------------------------------
  //@{

  /// Normal vector to the surface.
  /// @param mappedCoord The mapped coordinates where the Jacobian should be calculated
  /// @param result Storage for the resulting Jacobian matrix
  /// @note this is not the unit normal
  static void normal(const MappedCoordsT& mapped_coord, const NodesT& nodes, CoordsT& result);

  //@}

};

////////////////////////////////////////////////////////////////////////////////

} // LagrangeP1
} // Mesh
} // CF

#endif // CF_Mesh_LagrangeP1_Line2D_hpp