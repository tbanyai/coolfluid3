// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Mesh_ElementType_hpp
#define CF_Mesh_ElementType_hpp

////////////////////////////////////////////////////////////////////////////////

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/range.hpp>

#include "Math/MatrixTypes.hpp"

#include "Mesh/ShapeFunction.hpp"
#include "Mesh/GeoShape.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Mesh {

////////////////////////////////////////////////////////////////////////////////

/// This class represents the the data related to an ElementType
/// @author Tiago Quintino
/// @author Willem Deconinck
class Mesh_API ElementType : public Common::Component {

public: // typedefs

  typedef boost::shared_ptr< ElementType > Ptr;
  typedef boost::shared_ptr< ElementType const> ConstPtr;

public: // functions

  /// Stores connectivity information about the faces that form the cell boundary
  struct FaceConnectivity
  {
    /// Range of const indices
    typedef boost::iterator_range<std::vector<Uint>::const_iterator> RangeT;

    /// Index of the first node of each face, relative to the numbering of the parent cell
    std::vector<Uint> displs;

    /// Number of nodes for each face
    std::vector<Uint> stride;

    /// Node indices for each face
    std::vector<Uint> nodes;

    /// Iterator range over the nodes of the given face
    RangeT nodes_range(const Uint face) const
    {
      if(displs.empty())
        return boost::make_iterator_range(nodes.begin(), nodes.end());
      std::vector<Uint>::const_iterator begin = nodes.begin() + displs[face];
      return boost::make_iterator_range(begin, begin + stride[face]);
    }
  };

  /// @name Constructor / Destructor / Type name
  //  ------------------------------------------
  //@{

  /// Default constructor without arguments
  ElementType( const std::string& name ) : Common::Component(name) {}

  /// Default destructor
  virtual ~ElementType() {}

  static std::string type_name() { return "ElementType"; }

  // @}

  /// @name Accessor functions
  //  ------------------------
  //@{

  /// @return shape as string
  std::string shape_name() const { return Mesh::GeoShape::Convert::instance().to_str( m_shape ); }

  /// @return shape as enum
  GeoShape::Type shape() const  {  return m_shape; }

  /// @return number of faces
  Uint nb_faces() const  {  return m_nb_faces;  }

  /// @return number of edges
  Uint nb_edges() const  {  return m_nb_edges;  }

  /// @return m_nbNodes
  Uint nb_nodes() const  { return m_nb_nodes; }

  /// @return m_order
  Uint order() const { return m_order; }

  /// @return m_dimensionality
  Uint dimensionality() const { return m_dimensionality; }

  /// @return m_dimension
  Uint dimension() const { return m_dimension; }

  /// @return the shape function defining this geometric element
  virtual const ShapeFunction& shape_function() const = 0;

  /// @return the face connectivity information
  virtual const FaceConnectivity& faces() const = 0;

  /// @return the face type for the given face
  virtual const ElementType& face_type(const Uint face) const = 0;

  //@}

  /// @name Computation functions
  //  ---------------------------
  //@{

  /// Compute Mapped Coordinates, a.k.a. local coordinates
  /// @param coord        [in]  the coordinates to be mapped
  /// @param nodes        [in]  coordinates of the element nodes (nb_nodes x dimension)
  /// @return mapped_coord
  virtual RealVector mapped_coordinate(const RealVector& coord, const RealMatrix& nodes) const = 0;

  /// Compute Mapped Coordinates, a.k.a. local coordinates
  /// @param coord        [in]  the coordinates to be mapped
  /// @param nodes        [in]  coordinates of the element nodes (nb_nodes x dimension)
  /// @param mapped_coord [out] the result
  virtual void compute_mapped_coordinate(const RealVector& coord, const RealMatrix& nodes, RealVector& mapped_coord) const = 0;
  /// Compute the determinant of the jacobian
  /// @param mapped_coord [in] coordinates in mapped space (dimensionality x 1)
  /// @param nodes        [in] coordinates of the element nodes (nb_nodes x dimension)
  /// @return jacobian determinant
  virtual Real jacobian_determinant(const RealVector& mapped_coord, const RealMatrix& nodes) const = 0;

  /// Compute the jacobian of the transformation
  /// @param mapped_coord [in]  coordinates in mapped space (dimensionality x 1)
  /// @param nodes        [in]  coordinates of the element nodes (nb_nodes x dimension)
  /// @param jacobian     [out] jacobian (size = dimensionality x dimension)
  virtual void compute_jacobian(const RealVector& mapped_coord, const RealMatrix& nodes, RealMatrix& jacobian) const = 0;

  /// Compute the jacobian of the transformation
  /// @param mapped_coord [in] coordinates in mapped space (dimensionality x 1)
  /// @param nodes        [in] coordinates of the element nodes (nb_nodes x dimension)
  /// @return jacobian (size = dimensionality x dimension)
  virtual RealMatrix jacobian(const RealVector& mapped_coord, const RealMatrix& nodes) const = 0;

  /// Compute the adjoint of the jacobian of the transformation
  /// Useful for computation of inverse jacobian = 1/jacobian_determinant * jacobian_adjoint
  /// @param mapped_coord     [in]  coordinates in mapped space (dimensionality x 1)
  /// @param nodes            [in]  coordinates of the element nodes (nb_nodes x dimension)
  /// @param jacobian adjoint [out] jacobianadjoint (size = dimensionality x dimension)
  virtual void compute_jacobian_adjoint(const RealVector& mapped_coord, const RealMatrix& nodes, RealMatrix& jacobian_adjoint) const = 0;

  /// compute volume given coordinates
  /// @param nodes        [in] coordinates of the element nodes (nb_nodes x dimension)
  /// @note Only in elements of (dimensionality == dimension) will
  /// the volume be different from zero
  virtual Real volume(const RealMatrix& nodes) const = 0;

  /// compute area using given coordinates.
  /// @param nodes        [in] coordinates of the element nodes (nb_nodes x dimension)
  /// @note Only in elements of dimensionality == dimensionality-1
  /// will the area be different from zero
  virtual Real area(const RealMatrix& nodes) const = 0;

  /// compute the normal to the element. The direction will be taken according
  /// to the order of the coordinates
  virtual void compute_normal(const RealMatrix& nodes, RealVector& normal) const = 0;

  /// compute centroid of element given coordinates
  /// @param nodes        [in] coordinates of the element nodes (nb_nodes x dimension)
  virtual void compute_centroid(const RealMatrix& nodes , RealVector& centroid) const = 0;

  /// @return if the coordinate is in the element with given nodes
  /// @param [in] coord  the coordinates that will be checked
  /// @param [in] nodes  the nodes of the element
  virtual bool is_coord_in_element(const RealVector& coord, const RealMatrix& nodes) const = 0;

  /// Compute the jacobian of the plane or section of the element.
  /// The section is given by a mapped coordinate, and a direction perpendicular
  /// to the plane.
  /// Only elements with the dimension == dimensionality implement this function
  /// @param mapped_coord [in] coordinates in mapped space (dimensionality x 1)
  /// @param nodes        [in] coordinates of the element nodes (nb_nodes x dimension)
  /// @param orientation  [in] direction normal to the plane
  /// @return vector in mapped space scaled with the jacobian of the
  ///         section (not the volume).
  virtual RealVector plane_jacobian_normal(const RealVector& mapped_coord,
                                           const RealMatrix& nodes,
                                           const CoordRef orientation) const = 0;

  /// Compute the jacobian of the plane or section of the element.
  /// The section is given by a mapped coordinate, and a direction perpendicular
  /// to the plane.
  /// Only elements with the dimension == dimensionality implement this function
  /// @param mapped_coord [in]  coordinates in mapped space (dimensionality x 1)
  /// @param nodes        [in]  coordinates of the element nodes (nb_nodes x dimension)
  /// @param orientation  [in]  direction normal to the plane
  /// @param result       [out] vector in mapped space scaled with the jacobian of the
  ///                           section (not the volume).
  virtual void compute_plane_jacobian_normal(const RealVector& mapped_coord,
                                             const RealMatrix& nodes,
                                             const CoordRef orientation,
                                             RealVector& result) const = 0;

  //@}

protected: // data

  /// the GeoShape::Type corresponding to the shape
  GeoShape::Type m_shape;
  /// the number of nodes in this element type
  Uint m_nb_nodes;
  /// the  geometric order of this element
  Uint m_order;
  /// the dimension of the coordinates of this elements
  Uint m_dimension;
  /// the dimensionality of the element
  Uint m_dimensionality;
  /// number of edges
  Uint m_nb_edges;
  /// number of edges
  Uint m_nb_faces;

}; // ElementType

////////////////////////////////////////////////////////////////////////////////

template <typename ETYPE>
struct IsElementType
{
  /// Return true if etype is of concrete type ETYPE
  bool operator()(const ElementType& etype)
  {
    return dynamic_cast<ETYPE const*>(&etype) != nullptr;
  }
};

////////////////////////////////////////////////////////////////////////////////

} // Mesh
} // CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Mesh_ElementType_hpp
