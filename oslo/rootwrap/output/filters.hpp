#ifndef __FILTERS_HPP
#define __FILTERS_HPP

using namespace __shedskin__;
namespace __filters__ {

extern str *const_0, *const_1, *const_10, *const_11, *const_12, *const_13, *const_14, *const_15, *const_16, *const_17, *const_18, *const_19, *const_2, *const_20, *const_21, *const_22, *const_23, *const_24, *const_25, *const_26, *const_27, *const_28, *const_29, *const_3, *const_30, *const_31, *const_32, *const_4, *const_5, *const_6, *const_7, *const_8, *const_9;

class CommandFilter;
class RegExpFilter;
class PathFilter;
class KillFilter;
class ReadFileFilter;
class IpFilter;
class EnvFilter;
class ChainingFilter;
class IpNetnsExecFilter;


extern str *__name__;


extern class_ *cl_CommandFilter;
class CommandFilter : public pyobj {
/**
Command filter only checking that the 1st argument matches exec_path.
*/
public:

    str *name;
    str *run_as;
    str *real_exec;
    list<str *> *args;
    str *exec_path;

    CommandFilter() {}
    CommandFilter(str *exec_path, str *run_as, list<str *> *args_list) {
        this->__class__ = cl_CommandFilter;
        __init__(exec_path, run_as, args_list);
    }
    virtual list<str *> *exec_args(list<str *> *userargs);
    virtual list<str *> *get_command(list<str *> *userargs, list<str *> *exec_dirs);
    virtual __ss_bool  match(list<str *> *userargs);
    virtual __ss_bool  is_chaining_filter();
    str *get_exec(list<str *> *exec_dirs);
    void *__init__(str *exec_path, str *run_as, list<str *> *args_list);
};

extern class_ *cl_RegExpFilter;
class RegExpFilter : public CommandFilter {
/**
Command filter doing regexp matching for every argument.
*/
public:

    RegExpFilter() {}
    RegExpFilter(str *exec_path, str *run_as, list<str *> *args_list) {
        this->__class__ = cl_RegExpFilter;
        __init__(exec_path, run_as, args_list);
    }
    __ss_bool match(list<str *> *userargs);
};

extern class_ *cl_PathFilter;
class PathFilter : public CommandFilter {
/**
Command filter checking that path arguments are within given dirs

One can specify the following constraints for command arguments:
    1) pass     - pass an argument as is to the resulting command
    2) some_str - check if an argument is equal to the given string
    3) abs path - check if a path argument is within the given base dir

A typical rootwrapper filter entry looks like this:
    # cmdname: filter name, raw command, user, arg_i_constraint [, ...]
    chown: PathFilter, /bin/chown, root, nova, /var/lib/images

*/
public:

    PathFilter() {}
    PathFilter(str *exec_path, str *run_as, list<str *> *args_list) {
        this->__class__ = cl_PathFilter;
        __init__(exec_path, run_as, args_list);
    }
    list<str *> *get_command(list<str *> *userargs, list<str *> *exec_dirs);
    __ss_bool match(list<str *> *userargs);
};

extern class_ *cl_KillFilter;
class KillFilter : public CommandFilter {
/**
Specific filter for the kill calls.

1st argument is the user to run /bin/kill under
2nd argument is the location of the affected executable
    if the argument is not absolute, it is checked against $PATH
Subsequent arguments list the accepted signals (if any)

This filter relies on /proc to accurately determine affected
executable, so it will only work on procfs-capable systems (not OSX).
*/
public:

    KillFilter() {}
    KillFilter(str *run_as, list<str *> *args_list) {
        this->__class__ = cl_KillFilter;
        __init__(run_as, args_list);
    }
    void *__init__(str *run_as, list<str *> *args_list);
    __ss_bool match(list<str *> *userargs);
};

extern class_ *cl_ReadFileFilter;
class ReadFileFilter : public CommandFilter {
/**
Specific filter for the utils.read_file_as_root call.
*/
public:
    str *file_path;

    ReadFileFilter() {}
    ReadFileFilter(str *file_path, list<str *> *args_list) {
        this->__class__ = cl_ReadFileFilter;
        __init__(file_path, args_list);
    }
    void *__init__(str *file_path, list<str *> *args_list);
    __ss_bool match(list<str *> *userargs);
};

extern class_ *cl_IpFilter;
class IpFilter : public CommandFilter {
/**
Specific filter for the ip utility to that does not match exec.
*/
public:

    IpFilter() {}
    IpFilter(str *exec_path, str *run_as, list<str *> *args_list) {
        this->__class__ = cl_IpFilter;
        __init__(exec_path, run_as, args_list);
    }
    __ss_bool match(list<str *> *userargs);
};

extern class_ *cl_EnvFilter;
class EnvFilter : public CommandFilter {
/**
Specific filter for the env utility.

Behaves like CommandFilter, except that it handles
leading env A=B.. strings appropriately.
*/
public:

    EnvFilter() {}
    EnvFilter(str *exec_path, str *run_as, list<str *> *args_list) {
        this->__class__ = cl_EnvFilter;
        __init__(exec_path, run_as, args_list);
    }
    list<str *> *exec_args(list<str *> *userargs);
    void *__init__(str *exec_path, str *run_as, list<str *> *args_list);
    set<str *> *_extract_env(list<str *> *arglist);
    list<str *> *get_command(list<str *> *userargs, list<str *> *exec_dirs);
    __ss_bool match(list<str *> *userargs);
};

extern class_ *cl_ChainingFilter;
class ChainingFilter : public CommandFilter {
public:

    ChainingFilter() {}
    ChainingFilter(str *exec_path, str *run_as, list<str *> *args_list) {
        this->__class__ = cl_ChainingFilter;
        __init__(exec_path, run_as, args_list);
    }
    __ss_bool is_chaining_filter();
};

extern class_ *cl_IpNetnsExecFilter;
class IpNetnsExecFilter : public ChainingFilter {
/**
Specific filter for the ip utility to that does match exec.
*/
public:

    IpNetnsExecFilter() {}
    IpNetnsExecFilter(str *exec_path, str *run_as, list<str *> *args_list) {
        this->__class__ = cl_IpNetnsExecFilter;
        __init__(exec_path, run_as, args_list);
    }
    list<str *> *exec_args(list<str *> *userargs);
    __ss_bool match(list<str *> *userargs);
};

extern list<str *> * default_0;
extern list<str *> * default_18;
extern list<str *> * default_12;
extern void * default_9;
extern list<str *> * default_4;
extern list<str *> * default_14;
extern list<str *> * default_11;
extern list<str *> * default_19;
extern list<str *> * default_16;
extern list<str *> * default_6;
extern list<str *> * default_5;
extern list<str *> * default_17;
extern list<str *> * default_3;
extern list<str *> * default_8;
extern list<str *> * default_13;
extern list<str *> * default_15;
extern list<str *> * default_10;
extern list<str *> * default_1;
extern list<str *> * default_2;
extern list<str *> * default_7;

void __init();
CommandFilter *build_filter(str *class_name, list<str *> *arg_list);

} // module namespace
#endif
