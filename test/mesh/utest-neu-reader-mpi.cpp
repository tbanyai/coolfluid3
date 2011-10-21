// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test module for cf3::mesh::Neu::CReader"

#include <boost/test/unit_test.hpp>

#include "common/Log.hpp"
#include "common/Core.hpp"
#include "common/Root.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/CRegion.hpp"
#include "mesh/MeshReader.hpp"
#include "mesh/MeshWriter.hpp"
#include "mesh/MeshTransformer.hpp"

#include "mesh/CDynTable.hpp"
#include "mesh/CList.hpp"
#include "mesh/CTable.hpp"
#include "mesh/Geometry.hpp"

using namespace std;
using namespace boost;
using namespace cf3;
using namespace cf3::mesh;
using namespace cf3::common;

////////////////////////////////////////////////////////////////////////////////

struct NeuReaderMPITests_Fixture
{
  /// common setup for each test case
  NeuReaderMPITests_Fixture()
  {
    m_argc = boost::unit_test::framework::master_test_suite().argc;
    m_argv = boost::unit_test::framework::master_test_suite().argv;
  }

  /// common tear-down for each test case
  ~NeuReaderMPITests_Fixture()
  {
  }
  /// possibly common functions used on the tests below


  /// common values accessed by all tests goes here
  int    m_argc;
  char** m_argv;

};

////////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE( NeuReaderMPITests_TestSuite, NeuReaderMPITests_Fixture )

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( init_mpi )
{
  PE::Comm::instance().init(m_argc,m_argv);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( read_2d_mesh )
{

  MeshReader::Ptr meshreader = build_component_abstract_type<MeshReader>("CF.Mesh.Neu.CReader","meshreader");

  meshreader->configure_option("read_groups",true);

  // the mesh to store in
  Mesh& mesh = Core::instance().root().create_component<Mesh>("quadtriag");

  meshreader->read_mesh_into("quadtriag.neu",mesh);


  CFinfo << "elements count = " << find_component<CRegion>(mesh).recursive_elements_count() << CFendl;
  CFinfo << "nodes count    = " << find_component<CRegion>(mesh).recursive_nodes_count() << CFendl;

  Uint nb_ghosts=0;

  MeshWriter::Ptr gmsh_writer = build_component_abstract_type<MeshWriter>("CF.Mesh.Gmsh.CWriter","meshwriter");
  gmsh_writer->write_from_to(mesh,"quadtriag.msh");

  BOOST_CHECK(true);

  CFinfo << mesh.tree() << CFendl;

  Geometry& nodes = find_component_recursively<Geometry>(mesh);
  for (Uint n=0; n<nodes.size(); ++n)
  {
    if (nodes.is_ghost(n))
    {
      CFinfo << "node " << n << " is a ghost node" << CFendl;
      ++nb_ghosts;
    }
  }
  CFinfo << "ghost node count = " << nb_ghosts << CFendl;
}

////////////////////////////////////////////////////////////////////////////////
/*
BOOST_AUTO_TEST_CASE( threeD_test )
{

  MeshReader::Ptr meshreader = build_component_abstract_type<MeshReader>("CF.Mesh.Neu.CReader","meshreader");

  meshreader->configure_option("number_of_processors",(Uint) Comm::instance().size());
  meshreader->configure_option("rank",(Uint) Comm::instance().rank());
  meshreader->configure_option("Repartition",false);
  meshreader->configure_option("OutputRank",(Uint) 2);

  // the file to read from
  boost::filesystem::path fp_in ("hextet.neu");

  // the mesh to store in
  Mesh::Ptr mesh ( allocate_component<Mesh>  ( "mesh" ) );


  CFinfo.setFilterRankZero(false);
  meshreader->do_read_mesh_into(fp_in,mesh);
  CFinfo.setFilterRankZero(true);

  boost::filesystem::path fp_out ("hextet.msh");
  MeshWriter::Ptr gmsh_writer = build_component_abstract_type<MeshWriter>("CF.Mesh.Gmsh.CWriter","meshwriter");
  gmsh_writer->write_from_to(mesh,fp_out);

  BOOST_CHECK(true);

}
*/
////////////////////////////////////////////////////////////////////////////////
/*
BOOST_AUTO_TEST_CASE( read_multiple_2D )
{

  MeshReader::Ptr meshreader = build_component_abstract_type<MeshReader>("CF.Mesh.Neu.CReader","meshreader");

  meshreader->configure_option("Repartition",true);
  meshreader->configure_option("OutputRank",(Uint) 0);

  // the file to read from
  boost::filesystem::path fp_in ("quadtriag.neu");

  // the mesh to store in
  Mesh::Ptr mesh ( allocate_component<Mesh>  ( "mesh" ) );


  CFinfo.setFilterRankZero(false);



  for (Uint count=1; count<=2; ++count)
  {
    CFinfo << "\n\n\nMesh parallel:" << CFendl;
    meshreader->do_read_mesh_into(fp_in,mesh);
  }



  CFinfo.setFilterRankZero(true);
  CFinfo << mesh->tree() << CFendl;
  CFinfo << meshreader->tree() << CFendl;
  MeshTransformer::Ptr info  = build_component_abstract_type<MeshTransformer>("Info","info");
  info->transform(mesh);


  boost::filesystem::path fp_out ("quadtriag_mult.msh");
  MeshWriter::Ptr gmsh_writer = build_component_abstract_type<MeshWriter>("CF.Mesh.Gmsh.CWriter","meshwriter");
  gmsh_writer->write_from_to(mesh,fp_out);

  BOOST_CHECK_EQUAL(1,1);

}
*/
////////////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE( finalize_mpi )
{
  PE::Comm::instance().finalize();
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
