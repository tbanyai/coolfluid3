// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "common/Signal.hpp"
#include "common/Builder.hpp"
#include "common/OptionT.hpp"
#include "common/OptionArray.hpp"

#include "common/XML/SignalOptions.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/Region.hpp"
#include "mesh/Field.hpp"
#include "mesh/FieldManager.hpp"

#include "physics/PhysModel.hpp"

#include "solver/Solver.hpp"

#include "sdm/Tags.hpp"
#include "sdm/Init.hpp"

#include "InitialConditions.hpp"

using namespace cf3::common;
using namespace cf3::common::XML;
using namespace cf3::mesh;

namespace cf3 {
namespace sdm {


////////////////////////////////////////////////////////////////////////////////////////////

common::ComponentBuilder < InitialConditions, Action, LibSDM > InitialConditions_Builder;

////////////////////////////////////////////////////////////////////////////////////////////

InitialConditions::InitialConditions ( const std::string& name ) :
  cf3::solver::ActionDirector(name)
{
  mark_basic();

  // signals

  regist_signal( "create_initial_condition" )
      .connect  ( boost::bind( &InitialConditions::signal_create_initial_condition, this, _1 ) )
      .signature( boost::bind( &InitialConditions::signature_signal_create_initial_condition, this, _1))
      .description("creates an initial condition for the solution")
      .pretty_name("Create Initial Condition");
}



void InitialConditions::execute()
{
  // apply all registered actions

  ActionDirector::execute();

  /// @todo apply all strong BCs

//  Action& strong_bcs =
//      access_component( "cpath:../BoundaryConditions/StrongBCs" ).as_type<Action>();

//  strong_bcs.execute();

  // synchronize fields to insure consistency of parallel data

//  Action& synchronize =
//      access_component( "cpath:../actions/Synchronize" ).as_type<Action>();

//  synchronize.execute();
}

solver::Action& InitialConditions::create_initial_condition(const std::string& name, const std::vector<URI>& regions)
{
  Handle<solver::Action> ic = create_component< sdm::Init >(name);

  Handle<Field> solution = follow_link(solver().field_manager().get_child(sdm::Tags::solution()))->handle<Field>();

  ic->options().configure_option( sdm::Tags::solver() , m_solver);
  ic->options().configure_option( sdm::Tags::mesh(), m_mesh);
  ic->options().configure_option( "solution_field", solution);

  if( regions.empty() ) // if user did not specify, then use the one from the solver
  {
    ic->options().configure_option("regions" , solver().options().option("regions").value< std::vector<common::URI> >() );
  }
  else
  {
    ic->options().configure_option("regions" , regions);
  }

  ic->options().configure_option( sdm::Tags::physical_model() , m_physical_model);

  return *ic;
}

void InitialConditions::signal_create_initial_condition ( SignalArgs& args )
{
  SignalOptions options( args );

  std::string name = options.value<std::string>("name");

  std::vector<URI> regions;
  if( options.check("regions") )
  {
    regions = options.array<URI>("regions");
  }
  else // if user did not specify, then use the whole topology (all regions)
  {
    regions = solver().options().option("regions").value< std::vector<common::URI> >();
  }

  solver::Action& created_component = create_initial_condition(name,regions);

  SignalFrame reply = args.create_reply(uri());
  SignalOptions reply_options(reply);
  reply_options.add_option("created_component", created_component.uri());
}


void InitialConditions::signature_signal_create_initial_condition ( SignalArgs& args )
{
  SignalOptions options( args );

  // name

  options.add_option("name", std::string() )
      .description("Name for created initial condition" );

  // regions

  std::vector<URI> dummy;

  /// @todo create here the list of restricted regions, both volume and surface

  options.add_option("regions", dummy )
      .description("Regions where to apply the initial condition [optional]");
}

////////////////////////////////////////////////////////////////////////////////////////////


} // sdm
} // cf3
