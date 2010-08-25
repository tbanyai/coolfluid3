#ifndef CF_Common_OptionArray_hpp
#define CF_Common_OptionArray_hpp

////////////////////////////////////////////////////////////////////////////////

#include <boost/foreach.hpp>

#include "Common/Option.hpp"
#include "Common/BasicExceptions.hpp"

namespace CF {
namespace Common {

////////////////////////////////////////////////////////////////////////////////

  /// Class defines an array of options of the same type to be used by the ConfigObject class
  /// This class supports the following types:
  ///   - bool
  ///   - int
  ///   - CF:Uint
  ///   - CF::Real
  ///   - std::string
  /// @author Tiago Quintino
  template < typename TYPE >
      class OptionArray : public Option  {

  public:

    // (QG) : temporary fix, compilation fails at the FOREACH in
    // dump_to_str() function on MSVC if TYPE is bool with message
    // "cannot convert from 'std::_Vb_reference<_Sizet,_Difft,_MyContTy>' "
#if defined (_MSC_VER)
    typedef std::deque<TYPE> value_type;
#else
    typedef std::vector<TYPE> value_type;
#endif

    typedef TYPE element_type;

    OptionArray ( const std::string& name, const std::string& desc, const value_type& def );

    /// @name VIRTUAL FUNCTIONS
    //@{

    /// updates the option value using the xml configuration
    /// @param node XML node with data for this option
    virtual void change_value ( XmlNode& node );

    /// @returns the xml tag for this option
    virtual const char * tag() const { return "array"; }

    /// @returns the value as a sd::string
    virtual std::string value_str () const;

    /// @returns the default value as a sd::string
    virtual std::string def_str () const;

    //@} END VIRTUAL FUNCTIONS

  private: // helper functions

    void copy_to_linked_params ( const value_type& val );

    const char * type_tag() const;

    std::string dump_to_str ( const boost::any& c ) const;

  }; // OptionArray

////////////////////////////////////////////////////////////////////////////////

  template < typename TYPE>
  OptionArray<TYPE>::OptionArray ( const std::string& name, const std::string& desc, const value_type& def ) :
      Option(name,DEMANGLED_TYPEID(value_type), desc, def)
  {
//    CFinfo
//        << " creating OptionArray of " << () <<  "\'s [" << m_name << "]"
//        << " of type [" << m_type << "]"
//        << " w default [" << def_str() << "]"
//        << " w desc [" << m_description << "]\n"
//        << CFendl;
  }

  template < typename TYPE >
      void OptionArray<TYPE>::change_value ( XmlNode& node )
  {
    XmlAttr *attr = node.first_attribute( "type" );

    if ( !attr )
      throw ParsingFailed (FromHere(), "OptionArray does not have \'type\' attribute" );

    if ( strcmp(attr->value(),type_tag()) )
      throw ParsingFailed (FromHere(), "OptionArray expected \'type\' attribute \'"
                                       +  std::string(attr->value())
                                       + "\' but got \'"
                                       +  std::string(type_tag()) + "\'"  );

    value_type val; // empty vector

    for (XmlNode * itr = node.first_node(); itr ; itr = itr->next_sibling() )
    {
      TYPE vi;
      xmlstr_to_value(*itr,vi);
      val.push_back(vi);
    }

    XmlAttr *size_attr = node.first_attribute( "size" );
    if ( !size_attr )
      throw ParsingFailed (FromHere(), "OptionArray does not have \'size\' attribute" );

    Uint expected_size = 0;
    xmlstr_to_value(*size_attr,expected_size);
    if ( expected_size != val.size() )
      throw ParsingFailed (FromHere(), "OptionArray \'size\' did not match number of entries" );

    m_value = val;
    copy_to_linked_params(val);
  }

  template < typename TYPE >
      void OptionArray<TYPE>::copy_to_linked_params ( const value_type& val )
  {
    BOOST_FOREACH ( void* v, this->m_linked_params )
    {
      value_type* cv = static_cast<value_type*>(v);
      *cv = val;
    }
  }

  template < typename TYPE >
      std::string OptionArray<TYPE>::value_str () const
  {
    return dump_to_str(m_value);
  }

  template < typename TYPE >
      std::string OptionArray<TYPE>::def_str () const
  {
    return dump_to_str(m_default);
  }

    template < typename TYPE >
        std::string OptionArray<TYPE>::dump_to_str ( const boost::any& c ) const
  {
    value_type values = boost::any_cast<value_type>(c);
    std::string result;
    BOOST_FOREACH ( TYPE& v, values )
    {
      result += value_to_xmlstr ( v );
      result += ":";
    }
    if ( !result.empty() ) // remove last ":"
      result.erase(result.size()-1);

    return result;
  }


////////////////////////////////////////////////////////////////////////////////

} // Common
} // CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Common_OptionArray_hpp
