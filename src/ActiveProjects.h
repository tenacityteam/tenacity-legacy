/**********************************************************************

  Tenacity

  ActiveProjects.h

**********************************************************************/

#pragma once


#include "Identifier.h"

namespace ActiveProjects
{
   FilePaths GetAll();
   void Add(const FilePath &path);
   void Remove(const FilePath &path);
   wxString Find(const FilePath &path);
};
