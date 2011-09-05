// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/CBuilder.hpp"

#include "Mesh/ShapeFunctionT.hpp"
#include "Mesh/LagrangeP2/Triag.hpp"

namespace CF {
namespace Mesh {
namespace LagrangeP2 {

////////////////////////////////////////////////////////////////////////////////

Common::ComponentBuilder < ShapeFunctionT<Triag>, ShapeFunction, LibLagrangeP2 >
   Triag_Builder(LibLagrangeP2::library_namespace()+"."+Triag::type_name());

////////////////////////////////////////////////////////////////////////////////

Triag::ValueT Triag::value(const MappedCoordsT& mapped_coord)
{
  ValueT result;
  compute_value(mapped_coord,result);
  return result;
}

////////////////////////////////////////////////////////////////////////////////

Triag::GradientT Triag::gradient(const MappedCoordsT& mapped_coord)
{
  GradientT result;
  compute_gradient(mapped_coord,result);
  return result;
}

////////////////////////////////////////////////////////////////////////////////

void Triag::compute_value(const MappedCoordsT& mapped_coord, ValueT& result)
{
  const Real L0 = 1. - mapped_coord[KSI] - mapped_coord[ETA];
  const Real L1 = mapped_coord[KSI];
  const Real L2 = mapped_coord[ETA];

  result[0] = ( 2.*L0 - 1. ) * L0;
  result[1] = ( 2.*L1 - 1. ) * L1;
  result[2] = ( 2.*L2 - 1. ) * L2;
  result[3] = 4.*L0*L1;
  result[4] = 4.*L1*L2;
  result[5] = 4.*L2*L0;
}

////////////////////////////////////////////////////////////////////////////////

void Triag::compute_gradient(const MappedCoordsT& mapped_coord, GradientT& result)
{
  const Real L0 = 1. - mapped_coord[KSI] - mapped_coord[ETA];
  const Real L1 = mapped_coord[KSI];
  const Real L2 = mapped_coord[ETA];

  result(KSI, 0) = - (4.*L0-1.);
  result(ETA, 0) = - (4.*L0-1.);

  result(KSI, 1) =   (4.*L1-1.);
  result(ETA, 1) =   0.;

  result(KSI, 2) =   0.;
  result(ETA, 2) =   (4.*L2-1.);

  result(KSI, 3) =   4.*(L0-L1);
  result(ETA, 3) = - 4.*L1;

  result(KSI, 4) =   4.*L2;
  result(ETA, 4) =   4.*L1;

  result(KSI, 5) = - 4.*L2;
  result(ETA, 5) =   4.*(L0-L2);
}

////////////////////////////////////////////////////////////////////////////////

const RealMatrix& Triag::local_coordinates()
{
  static const RealMatrix loc_coord =
      (RealMatrix(nb_nodes, dimensionality) <<

       0.,  0.,
       1.,  0.,
       0.,  1.,
       0.5, 0.5,
       0.,  0.5

       ).finished();
  return loc_coord;
}

////////////////////////////////////////////////////////////////////////////////

} // LagrangeP2
} // Mesh
} // CF