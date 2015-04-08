/** @file gsXml.h

    @brief Provides declaration of input/output XML utilities struct.

    This file is part of the G+Smo library. 

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    
    Author(s): A. Mantzaflaris
*/

#pragma once

#include <gsCore/gsForwardDeclarations.h>
#include <gsCore/gsExport.h>

// Default memory sizes
// #define RAPIDXML_STATIC_POOL_SIZE  ( 64*1024 )
// #define RAPIDXML_DYNAMIC_POOL_SIZE ( 64*1024 )

#include <rapidxml/rapidxml.hpp>             // External file
#include <rapidxml/rapidxml_print.hpp>       // External file
//#include <rapidxml/rapidxml_utils.hpp>     // External file
//#include <rapidxml/rapidxml_iterators.hpp> // External file


/*
// Forward declare rapidxml structures
namespace rapidxml 
{
    template<class Ch> class xml_node;
    template<class Ch> class xml_attribute;
    template<class Ch> class xml_document;
}
*/

#define GSXML_COMMON_FUNCTIONS(obj)             \
    static bool has(gsXmlNode * node)           \
    { return firstByTag(tag(), node) != 0;}     \
    static bool hasAny(gsXmlNode * node)        \
    { return anyByTag(tag(), node) != 0;}       \
    static bool count(gsXmlNode * node)         \
    { return countByTag(tag(), node) != 0; }    \
    static obj * getFirst (gsXmlNode * node)    \
    { return get(firstByTag(tag(), node)); }    \
    static obj * getAny (gsXmlNode * node)      \
    { return get(anyByTag(tag(), node)); }      \
    static  obj * getId (gsXmlNode * node, int id) \
    { return getById< obj >(node, id); }

#define TMPLA2(t1,t2)    t1,t2
#define TMPLA3(t1,t2,t3) t1,t2,t3
#define FILE_PRECISION 16

namespace gismo {

namespace internal {

typedef rapidxml::xml_node<char>        gsXmlNode;
typedef rapidxml::xml_attribute<char>   gsXmlAttribute;
typedef rapidxml::xml_document<char>    gsXmlTree;


/// Generic get XML class: specializations provide implementation
template<class Object>
class gsXml
{
private:
    gsXml() { }// Disallow instantization
public:
    static std::string tag ();
    static std::string type ();
    static Object * get (gsXmlNode * node);
    static gsXmlNode * put (const Object & obj, gsXmlTree & data);

    // Common operations
    static bool     has      (gsXmlNode * node);
    static bool     count    (gsXmlNode * node);
    static Object * getFirst (gsXmlNode * node);
    static Object * getAny   (gsXmlNode * node);
    static Object * getId    (gsXmlNode * node, int id);
};

/// Helper to read an object by a given id value: 
/// \param node parent node, we check his children to get the given id
template<class Object>
Object * getById(gsXmlNode * node, const int & id)
{
    std::string tag = internal::gsXml<Object>::tag();
    for (gsXmlNode * child = node->first_node(tag.c_str());
         child; child = child->next_sibling(tag.c_str()))
    {
        if (  atoi(child->first_attribute("id")->value() ) == id )
            return internal::gsXml<Object>::get(child);
    }
    std::cerr<<"gsXmlUtils Warning: "<< internal::gsXml<Object>::tag() 
             <<" with id="<<id<<" not found.\n";
    return NULL;
}

/// Helper to allocate XML value
GISMO_EXPORT char * makeValue( const std::string & value, gsXmlTree & data);

/// Helper to allocate XML attribute
GISMO_EXPORT gsXmlAttribute *  makeAttribute( const std::string & name,
				              const std::string & value, gsXmlTree & data);

/// Helper to allocate XML attribute with unsigned int value
GISMO_EXPORT gsXmlAttribute *  makeAttribute( const std::string & name,
					           const unsigned & value, gsXmlTree & data);

/// Helper to allocate XML node
GISMO_EXPORT gsXmlNode *  makeNode( const std::string & name, gsXmlTree & data);

/// Helper to allocate XML node with value
GISMO_EXPORT gsXmlNode * makeNode( const std::string & name,
			             const std::string & value, gsXmlTree & data);

/// Helper to convert small unsigned to string
GISMO_EXPORT std::string to_string(const unsigned & i);

/// Helper to count the number of Objects (by tag) that exist in the
/// XML tree
GISMO_EXPORT int countByTag(const std::string & tag, gsXmlNode * root );

/// Helper to count the number of Objects (by name and type) that
/// exist in the XML tree
GISMO_EXPORT int  countByTagType(const std::string & tag, 
                                 const std::string & type, 
                                 gsXmlNode * root );

/// Helper to get the first object (by tag) if one exists in
/// the XML tree
GISMO_EXPORT gsXmlNode * firstByTag(const std::string & tag,
                                    gsXmlNode * root );

/// Helper to get the first object (by tag and type) if one exists in
/// the XML tree
GISMO_EXPORT gsXmlNode * firstByTagType(const std::string & tag,
                                         const std::string & type, 
                                         gsXmlNode * root );

// Helper which finds a node matching \a tag and \a type in the XML
// tree
//GISMO_EXPORT gsXmlNode * anyByTagType(const std::string & tag,
//                                       const std::string & type,
//                                       gsXmlNode * root );

/// Helper to get any object (by tag) if one exists in the XML tree
GISMO_EXPORT gsXmlNode * anyByTag(const std::string & tag,
                                  gsXmlNode * root );

/// Helper to allocate XML node with gsMatrix value
template<class T>
gsXmlNode * makeNode( const std::string & name, 
                      const gsMatrix<T> & value, gsXmlTree & data,
                      bool transposed = false );


/// Helper to fetch matrices
template<class T>
void getMatrixFromXml ( gsXmlNode * node, 
                        unsigned const & rows, 
                        unsigned const & cols, 
                        gsMatrix<T> & result );

/// Helper to insert matrices into XML
template<class T>
gsXmlNode * putMatrixToXml ( gsMatrix<T> const & mat, 
                             gsXmlTree & data, std::string name = "Matrix");

/// Helper to fetch sparse entries
template<class T>
void getSparseEntriesFromXml ( gsXmlNode * node, 
                               gsSparseEntries<T> & result );

/// Helper to insert sparse matrices into XML
template<class T>
gsXmlNode * putSparseMatrixToXml ( gsSparseMatrix<T> const & mat, 
                                   gsXmlTree & data, std::string name = "SparseMatrix");

}// end namespace internal

}// end namespace gismo

#ifndef GISMO_BUILD_LIB
#include GISMO_HPP_HEADER(gsXml.hpp)
#endif