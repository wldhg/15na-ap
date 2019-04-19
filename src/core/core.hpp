// Widh Jio
// core.hpp

#include "syaa.hpp"
#include "keras.hpp"
#include "ws.hpp"

#ifndef SYAA_CORE
#define SYAA_CORE

namespace core
{

void procFlagOption(std::string);
void procShortFlagOption(std::string);
void procDataOption(std::string, std::string);
void procShortDataOption(std::string, std::string);

void startSyaaAP(fdeep::model *, WSServer *);

} // namespace core

#endif
