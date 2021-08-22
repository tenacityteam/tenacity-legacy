/**********************************************************************

  Tenacity

  ProjectFileIORegistry.h

  Paul Licameli

**********************************************************************/

#pragma once

#include <functional>

class AudacityProject;
class XMLTagHandler;
class wxString;

namespace ProjectFileIORegistry {

// Type of functions returning objects that interpret a part of the saved XML
using TagHandlerFactory =
   std::function< XMLTagHandler *( AudacityProject & ) >;

// Typically statically constructed
struct TENACITY_DLL_API Entry{
   Entry( const wxString &tag, const TagHandlerFactory &factory );
};

TagHandlerFactory Lookup( const wxString &tag );

}
