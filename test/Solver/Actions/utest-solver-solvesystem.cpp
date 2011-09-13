// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test module for the CSolveSystem action"

#include <boost/assign/std/vector.hpp>
#include <boost/test/unit_test.hpp>


#include "Solver/Actions/CSolveSystem.hpp"

#include "Common/Core.hpp"
#include "Common/CRoot.hpp"

#include "Common/PE/CommPattern.hpp"
#include "Common/PE/CommWrapper.hpp"

#include "Math/MatrixTypes.hpp"
#include "Math/LSS/System.hpp"

using namespace boost::assign;

using namespace CF;
using namespace CF::Common;
using namespace CF::Common::PE;
using namespace CF::Solver;
using namespace CF::Solver::Actions;
using namespace CF::Math;

BOOST_AUTO_TEST_SUITE( SolveSystemSuite )

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( TestSolveSystem )
{
  Comm::instance().init(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);
  
  CRoot& root = Core::instance().root();
  CSolveSystem& solve_action = root.create_component<CSolveSystem>("solve_action");
  LSS::System& lss = root.create_component<LSS::System>("LSS");
  CommPattern& cp = root.create_component<CommPattern>("commpattern");
  
  std::vector<Uint> gid, conn, startidx, rnk;
  gid += 0,1,2,3,4,5,6,7,8,9;
  rnk += 0,0,0,0,0,0,0,0,0,0;
  conn += 0,2,1,2,2,7,3,8,4,5,5,2,6,0,7,1,8,7,9,8;
  startidx += 0,2,4,6,8,10,12,14,16,18,20;
  cp.insert("gid",gid,1,false);
  cp.setup(cp.get_child_ptr("gid")->as_ptr<Common::CommWrapper>(),rnk);
  
  lss.configure_option("solver", std::string("EmptyLSS"));
  lss.create(cp, 4u, conn, startidx);
  
  solve_action.configure_option("lss", lss.uri());
  solve_action.execute();
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
