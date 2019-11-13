/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// core.hpp

#include "15na.hpp"
#include "csi.hpp"
#include "ws.hpp"

#ifndef IRONA_CORE
#define IRONA_CORE

namespace core
{

void procFlagOption(std::string);
void procShortFlagOption(std::string);
void procDataOption(std::string, std::string);
void procShortDataOption(std::string, std::string);
void procAPName(std::string);

} // namespace core

#endif
