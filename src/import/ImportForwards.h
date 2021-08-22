//
//  ImportForwards.h
//  Audacity
//
//  Created by Paul Licameli on 8/10/16.
//
//

#pragma once

#include <vector>
#include <memory>

class ImportPlugin;
class UnusableImportPlugin;

using ImportPluginList =
   std::vector< ImportPlugin * >;
using UnusableImportPluginList =
   std::vector< std::unique_ptr<UnusableImportPlugin> >;
