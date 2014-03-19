#ifndef __CMD_HPP
#define __CMD_HPP

using namespace __shedskin__;

namespace __wrapper__ { /* XXX */
class RootwrapConfig;
class FilterMatchNotExecutable;
class NoFilterMatched;
}
namespace __filters__ { /* XXX */
class PathFilter;
class CommandFilter;
class IpFilter;
class EnvFilter;
class KillFilter;
class ChainingFilter;
class RegExpFilter;
class ReadFileFilter;
class IpNetnsExecFilter;
}
namespace __cmd__ {

extern str *const_0, *const_1, *const_2, *const_3, *const_4, *const_5, *const_6, *const_7, *const_8;



extern __ss_int RC_BADCONFIG, RC_NOCOMMAND, RC_NOEXECFOUND, RC_UNAUTHORIZED;
extern str *__name__;


extern __ss_bool  default_0;

void *_exit_error(str *execname, str *message, __ss_int errorcode, __ss_bool log);
void *__ss_main();

} // module namespace
#endif
