#ifndef __WRAPPER_HPP
#define __WRAPPER_HPP

using namespace __shedskin__;

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
namespace __wrapper__ {

extern str *const_0, *const_1, *const_10, *const_11, *const_12, *const_13, *const_2, *const_3, *const_4, *const_5, *const_6, *const_7, *const_8, *const_9;

class NoFilterMatched;
class FilterMatchNotExecutable;
class RootwrapConfig;

typedef __ss_bool (*lambda0)(str *);

extern str *__name__;


extern class_ *cl_NoFilterMatched;
class NoFilterMatched : public Exception {
/**
This exception is raised when no filter matched.
*/
public:
    void *msg;
    void *message;

    NoFilterMatched() {}
    NoFilterMatched(void *msg) {
        this->__class__ = cl_NoFilterMatched;
        __init__(msg);
    }
    static void __static__();
};

extern class_ *cl_FilterMatchNotExecutable;
class FilterMatchNotExecutable : public Exception {
/**
Raised when a filter matched but no executable was found.
*/
public:
    __filters__::CommandFilter *match;

    FilterMatchNotExecutable() {}
    FilterMatchNotExecutable(__filters__::CommandFilter *match) {
        this->__class__ = cl_FilterMatchNotExecutable;
        __init__(match);
    }
    void *__init__(__filters__::CommandFilter *match);
};

extern class_ *cl_RootwrapConfig;
class RootwrapConfig : public pyobj {
public:
    __ss_bool use_syslog_rfc_format;
    str *syslog_log_level;
    list<str *> *filters_path;
    __ss_bool use_syslog;
    list<str *> *exec_dirs;
    str *syslog_log_facility;

    RootwrapConfig() {}
    RootwrapConfig(__ConfigParser__::RawConfigParser *config) {
        this->__class__ = cl_RootwrapConfig;
        __init__(config);
    }
    void *__init__(__ConfigParser__::RawConfigParser *config);
};

extern __filters__::CommandFilter * default_0;
extern void * default_3;
extern list<str *> * default_1;
extern void * default_2;

void __init();
void *setup_syslog(str *execname, str *facility, str *level);
__filters__::CommandFilter *build_filter(str *class_name, list<str *> *arg_list);
list<__filters__::CommandFilter *> *load_filters(list<str *> *filters_path);
__ss_bool non_chain_filter(__filters__::CommandFilter *fltr, __filters__::CommandFilter *f);
__filters__::CommandFilter *match_filter(list<__filters__::CommandFilter *> *filter_list, list<str *> *userargs, list<str *> *exec_dirs);

} // module namespace
#endif
