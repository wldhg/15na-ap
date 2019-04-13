// Widh Jio
// opt.core.cpp

#include "core.hpp"

using std::string;
using std::cout;
using std::endl;

void showHelpMessage() {
  cout << endl;
  cout << bold << "poscas" << def << " [OPTION...] MODEL" << endl;
  cout << endl;
  cout << "[OPTION]" << endl;
  cout << "\t-p, --port   Set specific port which is used in websocket." << endl;
  cout << "\t             (Default: 7654)" << endl;
  cout << "\t-s, --site   Set specific site name which is used in" << endl;
  cout << "\t             websocket. (Default: localhost)" << endl;
  cout << "\t-h, --help   Show this help message." << endl;
  cout << endl;
  cout << "MODEL" << endl;
  cout << "\tKeras model used in the server. `(MODEL).h5` and `(MODEL).yml`" << endl;
  cout << "\tfiles must be in `model` directory." << endl;
  cout << endl;
}

void core::procFlagOption(string name)
{
  if (name.compare("help") == 0)
  {
    showHelpMessage();
    exit(0);
  }
  else
  {
    terminate("Unknown flag name. Is `=` missed?: " + name);
  }
}
void core::procShortFlagOption(string name)
{
  if (name.compare("h") == 0)
  {
    showHelpMessage();
    exit(0);
  }
  else
  {
    terminate("Unknown short flag name. Is `=` missed?: " + name);
  }
}

void core::procDataOption(string name, string value)
{
  if (name.compare("port") == 0)
  {
    setPort((unsigned short)atoi(value.c_str()));
  }
  else if (name.compare("site") == 0)
  {
    setSite(value);
  }
  else
  {
    terminate("Unknown data name: " + name);
  }
}
void core::procShortDataOption(string name, string value)
{
  if (name.compare("p") == 0)
  {
    setPort((unsigned short)atoi(value.c_str()));
  }
  else if (name.compare("s") == 0)
  {
    setSite(value);
  }
  else
  {
    terminate("Unknown short data name: " + name);
  }
}
