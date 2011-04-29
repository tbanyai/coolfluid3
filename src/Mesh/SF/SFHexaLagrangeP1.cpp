// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/CBuilder.hpp"

#include "LibSF.hpp"
#include "SFHexaLagrangeP1.hpp"

namespace CF {
namespace Mesh {
namespace SF {

////////////////////////////////////////////////////////////////////////////////

Common::ComponentBuilder < SFHexaLagrangeP1, ShapeFunction, LibSF > SFHexaLagrangeP1_Builder;

////////////////////////////////////////////////////////////////////////////////

SFHexaLagrangeP1::SFHexaLagrangeP1(const std::string& name) : ShapeFunction(name)
{
  m_dimensionality = dimensionality;
  m_nb_nodes = nb_nodes;
  m_order = order;
  m_shape = shape;
}

////////////////////////////////////////////////////////////////////////////////

void SFHexaLagrangeP1::value(const MappedCoordsT& mapped_coord, ValueT& result)
{
  const Real xi   = mapped_coord[KSI];
  const Real eta  = mapped_coord[ETA];
  const Real zeta = mapped_coord[ZTA];

  const Real a1 = (1 + xi);
  const Real a2 = (1 - xi);

  const Real b1 = (1 + eta);
  const Real b2 = (1 - eta);

  const Real c1 = (1 + zeta);
  const Real c2 = (1 - zeta);

  result[0] = a2*b2*c2;
  result[1] = a1*b2*c2;
  result[2] = a1*b1*c2;
  result[3] = a2*b1*c2;
  result[4] = a2*b2*c1;
  result[5] = a1*b2*c1;
  result[6] = a1*b1*c1;
  result[7] = a2*b1*c1;

  result *= 0.125;
}

////////////////////////////////////////////////////////////////////////////////

void SFHexaLagrangeP1::gradient(const MappedCoordsT& mapped_coord, GradientT& result)
{

  const Real xi   = mapped_coord[KSI];
  const Real eta  = mapped_coord[ETA];
  const Real zeta = mapped_coord[ZTA];

  const Real a1 = (1 + xi);
  const Real a2 = (1 - xi);

  const Real b1 = (1 + eta);
  const Real b2 = (1 - eta);

  const Real c1 = (1 + zeta);
  const Real c2 = (1 - zeta);

  result(KSI, 0) = -0.125 * b2*c2;
  result(ETA, 0) = -0.125 * a2*c2;
  result(ZTA, 0) = -0.125 * a2*b2;

  result(KSI, 1) =  0.125 * b2*c2;
  result(ETA, 1) = -0.125 * a1*c2;
  result(ZTA, 1) = -0.125 * a1*b2;

  result(KSI, 2) =  0.125 * b1*c2;
  result(ETA, 2) =  0.125 * a1*c2;
  result(ZTA, 2) = -0.125 * a1*b1;

  result(KSI, 3) = -0.125 * b1*c2;
  result(ETA, 3) =  0.125 * a2*c2;
  result(ZTA, 3) = -0.125 * a2*b1;

  result(KSI, 4) = -0.125 * b2*c1;
  result(ETA, 4) = -0.125 * a2*c1;
  result(ZTA, 4) =  0.125 * a2*b2;

  result(KSI, 5) =  0.125 * b2*c1;
  result(ETA, 5) = -0.125 * a1*c1;
  result(ZTA, 5) =  0.125 * a1*b2;

  result(KSI, 6) =  0.125 * b1*c1;
  result(ETA, 6) =  0.125 * a1*c1;
  result(ZTA, 6) =  0.125 * a1*b1;

  result(KSI, 7) = -0.125 * b1*c1;
  result(ETA, 7) =  0.125 * a2*c1;
  result(ZTA, 7) =  0.125 * a2*b1;
}

////////////////////////////////////////////////////////////////////////////////

SFHexaLagrangeP1::MappedNodesT SFHexaLagrangeP1::s_mapped_sf_nodes =  ( SFHexaLagrangeP1::MappedNodesT() <<
     -1., -1., -1.,
      1., -1., -1.,
      1.,  1., -1.,
     -1.,  1., -1.,
     -1., -1.,  1.,
      1., -1.,  1.,
      1.,  1.,  1.,
     -1.,  1.,  1.
).finished();

////////////////////////////////////////////////////////////////////////////////

} // SF
} // Mesh
} // CF
