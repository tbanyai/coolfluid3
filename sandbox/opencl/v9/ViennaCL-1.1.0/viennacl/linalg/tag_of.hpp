/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaCL - The Vienna Computing Library
                             -----------------
                            
   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Florian Rudolf                     flo.rudy+viennacl@gmail.com
               Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaCL base directory
======================================================================= */


#ifndef _VIENNACL_TAGOF_HPP_
#define _VIENNACL_TAGOF_HPP_

/** @file tag_of.hpp
    @brief Dispatch facility for distinguishing between ublas, STL and ViennaCL types
*/

#ifdef VIENNACL_HAVE_UBLAS
 #include <boost/numeric/ublas/vector.hpp>
 #include <boost/numeric/ublas/matrix.hpp>
#endif

namespace viennacl
{

  // ----------------------------------------------------
  // TAGS
  //
  struct tag_none     {};
  struct tag_ublas    {};
  struct tag_stl      {};
  struct tag_viennacl {};

  namespace traits
  {
    // ----------------------------------------------------
    // GENERIC BASE
    //
    /** @brief Generic base for wrapping other linear algebra packages
    * 
    *  Maps types to tags, e.g. viennacl::vector to tag_viennacl, ublas::vector to tag_ublas
    *  if the matrix type is unknown, tag_none is returned
    *
    *  This is an internal function only, there is no need for a library user of ViennaCL to care about it any further
    *
    * @tparam T   The type to be inspected
    */
    template< typename T, typename Active = void >
    struct tag_of;
   
    template < typename Sequence, typename Active >
    struct tag_of
    {
      typedef viennacl::tag_none  type;
    };
    
    
    #ifdef VIENNACL_HAVE_UBLAS
    // ----------------------------------------------------
    // UBLAS
    //
    template< typename T >
    struct tag_of< boost::numeric::ublas::vector<T> >
    {
      typedef viennacl::tag_ublas  type;
    };

    template< typename T >
    struct tag_of< boost::numeric::ublas::matrix<T> >
    {
      typedef viennacl::tag_ublas  type;
    };

    template< typename T1, typename T2 >
    struct tag_of< boost::numeric::ublas::matrix_unary2<T1,T2> >
    {
      typedef viennacl::tag_ublas  type;
    };

    template< typename T1, typename T2 >
    struct tag_of< boost::numeric::ublas::compressed_matrix<T1,T2> >
    {
      typedef viennacl::tag_ublas  type;
    };
    
    #endif

    // ----------------------------------------------------
    // STL types
    //
    
    //vector
    template< typename T, typename A >
    struct tag_of< std::vector<T, A> >
    {
      typedef viennacl::tag_stl  type;
    };

    //dense matrix
    template< typename T, typename A >
    struct tag_of< std::vector<std::vector<T, A>, A> >
    {
      typedef viennacl::tag_stl  type;
    };

    //sparse matrix (vector of maps)
    template< typename KEY, typename DATA, typename COMPARE, typename AMAP, typename AVEC>
    struct tag_of< std::vector<std::map<KEY, DATA, COMPARE, AMAP>, AVEC> >
    {
      typedef viennacl::tag_stl  type;
    };
    
    
    // ----------------------------------------------------
    // VIENNACL
    //
    template< typename T, unsigned int alignment >
    struct tag_of< viennacl::vector<T, alignment> >
    {
      typedef viennacl::tag_viennacl  type;
    };

    template< typename T, typename F, unsigned int alignment >
    struct tag_of< viennacl::matrix<T, F, alignment> >
    {
      typedef viennacl::tag_viennacl  type;
    };

    template< typename T1, typename T2, typename OP >
    struct tag_of< viennacl::matrix_expression<T1,T2,OP> >
    {
      typedef viennacl::tag_viennacl  type;
    };

    template< typename T, unsigned int I>
    struct tag_of< viennacl::compressed_matrix<T,I> >
    {
      typedef viennacl::tag_viennacl  type;
    };

    template< typename T, unsigned int I>
    struct tag_of< viennacl::coordinate_matrix<T,I> >
    {
      typedef viennacl::tag_viennacl  type;
    };
    // ----------------------------------------------------
  } // end namespace traits

  /** @brief Meta function which checks whether a tag is tag_ublas 
  *
  *  This is an internal function only, there is no need for a library user of ViennaCL to care about it any further
  */
  template <typename Tag>
  struct is_ublas
  {
     enum { value = false };  
  };

  template <>
  struct is_ublas< viennacl::tag_ublas >
  {
     enum { value = true };  
  };

  /** @brief Meta function which checks whether a tag is tag_ublas 
  *
  *  This is an internal function only, there is no need for a library user of ViennaCL to care about it any further
  */
  template <typename Tag>
  struct is_stl
  {
     enum { value = false };  
  };

  template <>
  struct is_stl< viennacl::tag_stl >
  {
     enum { value = true };  
  };

  
  /** @brief Meta function which checks whether a tag is tag_viennacl
  *
  *  This is an internal function only, there is no need for a library user of ViennaCL to care about it any further
  */
  template <typename Tag>
  struct is_viennacl
  {
     enum { value = false };  
  };

  template <>
  struct is_viennacl< viennacl::tag_viennacl >
  {
     enum { value = true };  
  };

} // end namespace viennacl

#endif
