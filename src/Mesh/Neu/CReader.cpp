#include <boost/foreach.hpp>
#include <boost/algorithm/string/erase.hpp>
#include "Common/ObjectProvider.hpp"
#include "Mesh/Neu/CReader.hpp"

#include "Mesh/CMesh.hpp"
#include "Mesh/CArray.hpp"
#include "Mesh/CRegion.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Mesh {
namespace Neu {
  
////////////////////////////////////////////////////////////////////////////////

CF::Common::ObjectProvider < Mesh::Neu::CReader,
                             Mesh::CMeshReader,
                             Mesh::Neu::NeuLib,
                             1 >
aNeuReader_Provider ( "Neu" );

//////////////////////////////////////////////////////////////////////////////

CReader::CReader( const CName& name )
: CMeshReader(name)
{
  build_component(this);

  m_supported_types.reserve(2);
  m_supported_types.push_back("P1-Quad2D");
  m_supported_types.push_back("P1-Triag2D");
  m_supported_types.push_back("P1-Hexa3D");
}

//////////////////////////////////////////////////////////////////////////////

void CReader::read_from_to(boost::filesystem::path& fp, const CMesh::Ptr& mesh)
{

  // if the file is present open it
  boost::filesystem::fstream file;
  if( boost::filesystem::exists(fp) )
  {
    CFLog(VERBOSE, "Opening file " <<  fp.string() << "\n");
    file.open(fp,std::ios_base::in); // exists so open it
  }
  else // doesnt exist so throw exception
  {
     throw boost::filesystem::filesystem_error( fp.string() + " does not exist",
                                                boost::system::error_code() );
  }

  // set the internal mesh pointer
  m_mesh = mesh;

  // must be in correct order!
  read_headerData(file);
  read_coordinates(file);
  read_connectivity(file);
  read_groups(file);

  file.close();
}

//////////////////////////////////////////////////////////////////////////////

void CReader::read_headerData(std::fstream& file)
{
  CFAUTOTRACE;
  Uint NUMNP, NELEM, NGRPS, NBSETS, NDFCD, NDFVL;
  std::string line;

  // skip 6 initial lines
  for (Uint i=0; i<6; ++i)
    getline(file,line);

  // read number of points, elements, groups, sets, dimensions, velocitycomponents
  getline(file,line);
  std::stringstream ss(line);
  ss >> NUMNP >> NELEM >> NGRPS >> NBSETS >> NDFCD >> NDFVL;

  m_headerData.NUMNP  = NUMNP;
  m_headerData.NELEM  = NELEM;
  m_headerData.NGRPS  = NGRPS;
  m_headerData.NBSETS = NBSETS;
  m_headerData.NDFCD  = NDFCD;
  m_headerData.NDFVL  = NDFVL;
  m_headerData.print();
  
  getline(file,line);
}

//////////////////////////////////////////////////////////////////////////////

void CReader::read_coordinates(std::fstream& file)
{
  CFAUTOTRACE;
  // Create the coordinates array
  m_mesh->create_array("coordinates");
  // create pointers to the coordinates array
  CArray::Ptr coordinates = m_mesh->get_component<CArray>("coordinates");
  // set dimension
  coordinates->initialize(m_headerData.NDFCD);
  // create a buffer to interact with coordinates
  CArray::Buffer buffer = coordinates->create_buffer(m_headerData.NUMNP);
  
  std::string line;
  // skip one line
  getline(file,line);

  // declare and allocate one coordinate row
  std::vector<Real> rowVector(m_headerData.NDFCD);

  for (Uint i=0; i<m_headerData.NUMNP; ++i) {
    getline(file,line);
    std::stringstream ss(line);
    Uint nodeNumber;
    ss >> nodeNumber;
    nodeNumber--;
    for (Uint dim=0; dim<m_headerData.NDFCD; ++dim)
      ss >> rowVector[dim];
    buffer.add_row(rowVector);
  }
  buffer.flush();
  
  getline(file,line);
}

//////////////////////////////////////////////////////////////////////////////

void CReader::read_connectivity(std::fstream& file)
{
  CFAUTOTRACE;
  // make temporary regions for each element type possible
  CRegion::Ptr tmp = m_mesh->create_region("tmp");

  std::map<std::string,boost::shared_ptr<CTable::Buffer> > buffer =
      create_leaf_regions_with_buffermap(tmp,m_supported_types);

  // skip next line
  std::string line;
  getline(file,line);
    
  // read every line and store the connectivity in the correct region through the buffer
  std::string etype_CF;
  for (Uint i=0; i<m_headerData.NELEM; ++i) {
    // element description
    Uint elementNumber, elementType, nbElementNodes;
    file >> elementNumber >> elementType >> nbElementNodes;
    elementNumber--;
    if      (elementType==2 && nbElementNodes==4) etype_CF = "P1-Quad2D";  // quadrilateral
    else if (elementType==3 && nbElementNodes==3) etype_CF = "P1-Triag2D"; // triangle
    else if (elementType==4 && nbElementNodes==8) etype_CF = "P1-Hexa3D";  // brick
    /// @todo to be implemented
    // else if (elementType==5 && nbElementNodes==6) ;// wedge (prism)
    // else if (elementType==6 && nbElementNodes==4) ;// tetrahedron
    // else if (elementType==7 && nbElementNodes==5) ;// pyramid
    else {
      CFerr << "error: no support for element type/nodes " << elementType << "/" << nbElementNodes << CFflush;
    }
    
    // get element nodes
    std::vector<Uint> rowVector(nbElementNodes);
    
    for (Uint j=0; j<nbElementNodes; ++j)
    {
      file >> rowVector[j];
      rowVector[j]--;
    }
    Uint table_idx = buffer[etype_CF]->get_total_nbRows();
    buffer[etype_CF]->add_row(rowVector);
    m_global_to_tmp.push_back(Region_TableIndex_pair(tmp->get_component<CRegion>(etype_CF),table_idx));
    
    // finish the line
    getline(file,line);
  }
  getline(file,line);  // ENDOFSECTION
}

//////////////////////////////////////////////////////////////////////////////

void CReader::read_groups(std::fstream& file)
{
  CFAUTOTRACE;
  std::string line;
  int dummy;
  
  CRegion::Ptr regions = m_mesh->create_region("regions");
  
  std::vector<GroupData> groups(m_headerData.NGRPS);
  for (Uint g=0; g<m_headerData.NGRPS; ++g) {    
    std::string ELMMAT;
    Uint NGP, NELGP, MTYP, NFLAGS, I;
    getline(file,line);  // ELEMENT GROUP...
    CFinfo << "line = " << line << CFendl;
    file >> line >> NGP >> line >> NELGP >> line >> MTYP >> line >> NFLAGS >> ELMMAT;
    groups[g].NGP    = NGP;
    groups[g].NELGP  = NELGP;
    groups[g].MTYP   = MTYP;
    groups[g].NFLAGS = NFLAGS;
    groups[g].ELMMAT = ELMMAT;
    groups[g].print();
    
    for (Uint i=0; i<NFLAGS; ++i)
      file >> dummy;
      
    groups[g].ELEM.reserve(NELGP);
    for (Uint i=0; i<NELGP; ++i) 
    {
      file >> I;
      groups[g].ELEM.push_back(I-1);     // set element index
    }
    getline(file,line);  // finish the line (read new line)
    getline(file,line);  // ENDOFSECTION
  }
  
  // 2 cases:
  // 1) there is only one group --> The tmp region can just be renamed
  //    and put in the filesystem as subcomponent of "mesh/regions"
  if (m_headerData.NGRPS == 1)
  {
    Component::Ptr tmp = m_mesh->remove_component("tmp");
    tmp->rename(groups[0].ELMMAT);
    regions->add_component(tmp);
  }
  // 2) there are multiple groups --> New regions have to be created
  //    and the elements from the tmp region have to be distributed among
  //    these new regions.
  else
  {
    // Create Region for each group
    BOOST_FOREACH(GroupData& group, groups)
    {

      CRegion::Ptr region = regions->create_region(group.ELMMAT);

      // Create regions for each element type in each group-region
      std::map<std::string,boost::shared_ptr<CTable::Buffer> > buffer =
          create_leaf_regions_with_buffermap(region,m_supported_types);

      // Copy elements from tmp_region in the correct region
      BOOST_FOREACH(Uint global_element, group.ELEM)
      {
        CRegion::Ptr tmp_region = m_global_to_tmp[global_element].first;
        Uint local_element = m_global_to_tmp[global_element].second;
        buffer[tmp_region->name()]->add_row(tmp_region->get_component<CTable>("table")->get_table()[local_element]);
      }
    }
    
    // Remove tmp region from component
    Component::Ptr tmp = m_mesh->remove_component("tmp");
    
    /// @todo check if the following really deletes it from memory
    tmp.reset();
    
  }

  // truely deallocate this vector
  std::vector<Region_TableIndex_pair>().swap (m_global_to_tmp);
  
  // Remove regions with empty connectivity tables
  remove_empty_leaf_regions(regions);
}

//////////////////////////////////////////////////////////////////////////////

} // Neu
} // Mesh
} // CF
