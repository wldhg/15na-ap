// Widh Jio
// core.hpp

#include "poscas.hpp"
#include "keras.hpp"
#include "ws.hpp"

#ifndef POSCA_CORE
#define POSCA_CORE

namespace core
{

void procFlagOption(std::string);
void procShortFlagOption(std::string);
void procDataOption(std::string, std::string);
void procShortDataOption(std::string, std::string);

void startPoscas(fdeep::model*, WSServer*);

} // namespace core

#endif
