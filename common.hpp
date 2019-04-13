// Widh Jio
// common.hpp

#include <iostream>
#include <cstdlib>
#include <string>

#ifndef POSCA_COMMON
#define POSCA_COMMON

inline std::string c2str(char *chars)
{
  std::string str(chars);
  return str;
}

inline std::ostream &bold(std::ostream &o) { return o << "\e[1m"; }
inline std::ostream &red(std::ostream &o) { return o << "\e[31m"; }
inline std::ostream &green(std::ostream &o) { return o << "\e[32m"; }
inline std::ostream &yellow(std::ostream &o) { return o << "\e[33m"; }
inline std::ostream &blue(std::ostream &o) { return o << "\e[34m"; }
inline std::ostream &pink(std::ostream &o) { return o << "\e[35m"; }
inline std::ostream &cyan(std::ostream &o) { return o << "\e[36m"; }
inline std::ostream &gray(std::ostream &o) { return o << "\e[37m"; }
inline std::ostream &def(std::ostream &o) { return o << "\e[0m"; }

#define $info std::cout << bold << cyan << "[Info] " << def
#define $warn std::cout << bold << yellow << "[Warn] " << def
#define $debug std::cout << bold << blue << "[Debug] " << def
#define $success std::cout << bold << green << "[Success] " << def
#define $error std::cerr << bold << red << "[Error] " << def

void _terminate();
void terminate(char *);
void terminate(char const *);
void terminate(std::string);

void setPort(unsigned short);
void setSite(std::string);
extern unsigned short port;
extern std::string site;

extern std::string model;

#endif
