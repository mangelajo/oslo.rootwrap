#include "builtin.hpp"
#include "os/__init__.hpp"
#include "os/path.hpp"
#include "re.hpp"
#include "stat.hpp"
#include "filters.hpp"

namespace __filters__ {

str *const_0, *const_1, *const_10, *const_11, *const_12, *const_13, *const_14, *const_15, *const_16, *const_17, *const_18, *const_19, *const_2, *const_20, *const_21, *const_22, *const_23, *const_24, *const_25, *const_26, *const_27, *const_28, *const_29, *const_3, *const_30, *const_31, *const_32, *const_4, *const_5, *const_6, *const_7, *const_8, *const_9;


str *__name__;


list<str *> * default_0;
list<str *> * default_18;
list<str *> * default_12;
void * default_9;
list<str *> * default_4;
list<str *> * default_14;
list<str *> * default_11;
list<str *> * default_19;
list<str *> * default_16;
list<str *> * default_6;
list<str *> * default_5;
list<str *> * default_17;
list<str *> * default_3;
list<str *> * default_8;
list<str *> * default_13;
list<str *> * default_15;
list<str *> * default_10;
list<str *> * default_1;
list<str *> * default_2;
list<str *> * default_7;

class list_comp_0 : public __iter<__ss_bool> {
public:
    list<str *> *__31, *__32;
    tuple2<str *, str *> *__27;
    __ss_bool __35, __36;
    list<tuple2<str *, str *> *>::for_in_loop __34;
    str *arg, *value;
    list<tuple2<str *, str *> *> *__28;
    __iter<tuple2<str *, str *> *> *__29;
    __ss_int __30, __33;

    PathFilter *self;
    list<str *> *arguments;
    int __last_yield;

    list_comp_0(PathFilter *self, list<str *> *arguments);
    __ss_bool __get_next();
};

class list_comp_1 : public __iter<__ss_bool> {
public:
    list<str *> *__41, *__42;
    list<tuple2<str *, str *> *>::for_in_loop __44;
    tuple2<str *, str *> *__37;
    str *arg, *value;
    list<tuple2<str *, str *> *> *__38;
    __iter<tuple2<str *, str *> *> *__39;
    __ss_int __40, __43;

    PathFilter *self;
    list<str *> *arguments;
    int __last_yield;

    list_comp_1(PathFilter *self, list<str *> *arguments);
    __ss_bool __get_next();
};

static inline list<str *> *list_comp_2(PathFilter *self, list<str *> *arguments);

list_comp_0::list_comp_0(PathFilter *self, list<str *> *arguments) {
    this->self = self;
    this->arguments = arguments;
    __last_yield = -1;
}

__ss_bool list_comp_0::__get_next() {
    if(!__last_yield) goto __after_yield_0;
    __last_yield = 0;

    FOR_IN_ZIP(arg,value,self->args,arguments,31,32,30,33)
        if (__NOT(__os__::__path__::isabs(arg))) {
            __result = __OR(___bool(__eq(arg, const_0)), ___bool(__eq(arg, value)), 35);
            return __result;
            __after_yield_0:;
        }
    END_FOR

    __stop_iteration = true;
}

list_comp_1::list_comp_1(PathFilter *self, list<str *> *arguments) {
    this->self = self;
    this->arguments = arguments;
    __last_yield = -1;
}

__ss_bool list_comp_1::__get_next() {
    if(!__last_yield) goto __after_yield_0;
    __last_yield = 0;

    FOR_IN_ZIP(arg,value,self->args,arguments,41,42,40,43)
        if (__os__::__path__::isabs(arg)) {
            __result = ___bool(__eq(__os__::__path__::commonprefix((new list<str *>(2,arg,__os__::__path__::realpath(value)))), arg));
            return __result;
            __after_yield_0:;
        }
    END_FOR

    __stop_iteration = true;
}

static inline list<str *> *list_comp_2(PathFilter *self, list<str *> *arguments) {
    list<str *> *__55, *__56;
    list<tuple2<str *, str *> *>::for_in_loop __58;
    tuple2<str *, str *> *__51;
    str *arg, *value;
    list<tuple2<str *, str *> *> *__52;
    __iter<tuple2<str *, str *> *> *__53;
    __ss_int __54, __57;

    list<str *> *__ss_result = new list<str *>();

    FOR_IN_ZIP(arg,value,self->args,arguments,55,56,54,57)
        __ss_result->append(((__os__::__path__::isabs(arg))?(__os__::__path__::realpath(value)):(value)));
    END_FOR

    return __ss_result;
}

/**
class CommandFilter
*/

class_ *cl_CommandFilter;

__ss_bool CommandFilter::is_chaining_filter() {
    
    return False;
}

str *CommandFilter::get_exec(list<str *> *exec_dirs) {
    /**
    Returns existing executable, or empty string if none found.
    */
    list<str *>::for_in_loop __6;
    list<str *> *__3;
    str *binary_path, *expanded_path;
    __ss_int __5;
    __iter<str *> *__4;

    if ((this->real_exec!=NULL)) {
        return this->real_exec;
    }
    this->real_exec = const_1;
    if (__os__::__path__::isabs(this->exec_path)) {
        if (__os__::access(this->exec_path, __os__::__ss_X_OK)) {
            this->real_exec = this->exec_path;
        }
    }
    else {

        FOR_IN(binary_path,exec_dirs,3,5,6)
            expanded_path = __os__::__path__::join(2, binary_path, this->exec_path);
            if (__os__::access(expanded_path, __os__::__ss_X_OK)) {
                this->real_exec = expanded_path;
                break;
            }
        END_FOR

    }
    return this->real_exec;
}

void *CommandFilter::__init__(str *exec_path, str *run_as, list<str *> *args_list) {
    
    this->name = const_1;
    this->exec_path = exec_path;
    this->run_as = run_as;
    this->args = args_list;
    this->real_exec = NULL;
    return NULL;
}

list<str *> *CommandFilter::exec_args(list<str *> *userargs) {
    
    return (new list<str *>(1,const_1));
}

list<str *> *CommandFilter::get_command(list<str *> *userargs, list<str *> *exec_dirs) {
    /**
    Returns command to execute (with sudo -u if run_as != root).
    */
    str *__10, *__9, *to_exec;

    to_exec = __OR(this->get_exec(exec_dirs), this->exec_path, 9);
    if (__ne(this->run_as, const_2)) {
        return ((new list<str *>(4,const_3,const_4,this->run_as,to_exec)))->__add__(userargs->__slice__(1, 1, 0, 0));
    }
    return ((new list<str *>(1,to_exec)))->__add__(userargs->__slice__(1, 1, 0, 0));
}

__ss_bool CommandFilter::match(list<str *> *userargs) {
    /**
    Only check that the first argument (command) matches exec_path.
    */
    __ss_bool __7, __8;

    return __AND(___bool((len(userargs)!=0)), ___bool(__eq(__os__::__path__::basename(this->exec_path), userargs->__getfast__(0))), 7);
}


/**
class RegExpFilter
*/

class_ *cl_RegExpFilter;

__ss_bool RegExpFilter::match(list<str *> *userargs) {
    list<str *> *__17, *__18;
    list<tuple2<str *, str *> *>::for_in_loop __20;
    __ss_bool __11, __12;
    tuple2<str *, str *> *__13;
    str *arg, *pattern;
    list<tuple2<str *, str *> *> *__14;
    __iter<tuple2<str *, str *> *> *__15;
    __ss_int __16, __19, __21;

    if ((__NOT(___bool(userargs)) or (len(this->args)!=len(userargs)))) {
        return False;
    }

    __21 = 0;
    FOR_IN_ZIP(pattern,arg,this->args,userargs,17,18,16,19)
        try {
            if (__NOT(___bool(__re__::match((pattern)->__add__(const_5), arg, 0)))) {
                __21 = 1;
                break;
            }
        } catch (__re__::error *) {
            return False;
        }
    END_FOR
    if (!__21) {
        return True;
    }

    return False;
}

/**
class PathFilter
*/

class_ *cl_PathFilter;

list<str *> *PathFilter::get_command(list<str *> *userargs, list<str *> *exec_dirs) {
    list<str *> *__50, *args, *arguments;
    str *__49, *command;

    __49 = userargs->__getfast__(0);
    __50 = userargs->__slice__(1, 1, 0, 0);
    command = __49;
    arguments = __50;
    args = list_comp_2(this, arguments);
    return CommandFilter::get_command(((new list<str *>(1,command)))->__add__(args), exec_dirs);
}

__ss_bool PathFilter::match(list<str *> *userargs) {
    list<str *> *__26, *arguments;
    str *__25, *command;
    __ss_bool __23, __24, __45, __46, __47, __48, args_equal_or_pass, equal_args_num, exec_is_valid, paths_are_within_base_dirs;

    if ((__NOT(___bool(userargs)) or (len(userargs)<2))) {
        return False;
    }
    __25 = userargs->__getfast__(0);
    __26 = userargs->__slice__(1, 1, 0, 0);
    command = __25;
    arguments = __26;
    equal_args_num = ___bool((len(this->args)==len(arguments)));
    exec_is_valid = CommandFilter::match(userargs);
    args_equal_or_pass = all(new list_comp_0(this, arguments));
    paths_are_within_base_dirs = all(new list_comp_1(this, arguments));
    return __AND(equal_args_num, __AND(exec_is_valid, __AND(args_equal_or_pass, paths_are_within_base_dirs, 47), 46), 45);
}

/**
class KillFilter
*/

class_ *cl_KillFilter;

void *KillFilter::__init__(str *run_as, list<str *> *args_list) {
    
    CommandFilter::__init__(const_6, run_as, args_list);
    return NULL;
}

__ss_bool KillFilter::match(list<str *> *userargs) {
    list<str *> *args;
    str *command, *kill_command, *signal;
    __ss_bool __59, __60, __63, __64, __65;

    if ((__NOT(___bool(userargs)) or __ne(userargs->__getfast__(0), const_7))) {
        return False;
    }
    args = (new list<str *>(userargs));
    if ((len(args)==3)) {
        signal = args->pop(1);
        if ((!((this->args)->__slice__(1, 1, 0, 0))->__contains__(signal))) {
            return False;
        }
    }
    else {
        if ((len(args)!=2)) {
            return False;
        }
        if ((len(this->args)>1)) {
            return False;
        }
    }
    try {
        command = __os__::readlink(__modct(const_8, 1, ___box(__int(args->__getfast__(1)))));
    } catch (ValueError *) {
        return False;
    } catch (OSError *) {
        return False;
    }
    command = (command->partition(const_9))->__getfast__(0);
    if (command->endswith(const_10)) {
        command = command->__slice__(2, 0, (-len(const_10)), 0);
    }
    kill_command = (this->args)->__getfast__(0);
    if (__os__::__path__::isabs(kill_command)) {
        return ___bool(__eq(kill_command, command));
    }
    return __AND(__os__::__path__::isabs(command), __AND(___bool(__eq(kill_command, __os__::__path__::basename(command))), ___bool((((__os__::__ss_environ)->get(const_11, const_1))->split(const_12))->__contains__(__os__::__path__::dirname(command))), 64), 63);
}

/**
class ReadFileFilter
*/

class_ *cl_ReadFileFilter;

void *ReadFileFilter::__init__(str *file_path, list<str *> *args_list) {
    
    this->file_path = file_path;
    CommandFilter::__init__(const_13, const_2, args_list);
    return NULL;
}

__ss_bool ReadFileFilter::match(list<str *> *userargs) {
    
    return ___bool(__eq(userargs, (new list<str *>(2,const_14,this->file_path))));
}

/**
class IpFilter
*/

class_ *cl_IpFilter;

__ss_bool IpFilter::match(list<str *> *userargs) {
    
    if (__eq(userargs->__getfast__(0), const_15)) {
        if (__eq(userargs->__getfast__(1), const_16)) {
            return ___bool(((new tuple2<str *, str *>(3,const_17,const_18,const_19)))->__contains__(userargs->__getfast__(2)));
        }
        else {
            return True;
        }
    }
    return False;
}

/**
class EnvFilter
*/

class_ *cl_EnvFilter;

list<str *> *EnvFilter::exec_args(list<str *> *userargs) {
    list<str *> *args;
    __ss_bool __75, __76;

    args = userargs->__slice__(0, 0, 0, 0);
    if (__eq(args->__getfast__(0), const_20)) {
        args->pop(0);
    }

    while ((___bool(args) and (args->__getfast__(0))->__contains__(const_21))) {
        args->pop(0);
    }
    return args;
}

void *EnvFilter::__init__(str *exec_path, str *run_as, list<str *> *args_list) {
    set<str *> *env_list;
    __ss_bool __70, __71;

    CommandFilter::__init__(exec_path, run_as, args_list);
    env_list = this->_extract_env(this->args);
    if (((exec_path)->__contains__(const_20) and (len(env_list)<len(this->args)))) {
        this->exec_path = (this->args)->__getfast__(len(env_list));
    }
    return NULL;
}

set<str *> *EnvFilter::_extract_env(list<str *> *arglist) {
    /**
    Extract all leading NAME=VALUE arguments from arglist.
    */
    list<str *> *__66;
    list<str *>::for_in_loop __69;
    str *arg;
    __iter<str *> *__67;
    set<str *> *envs;
    __ss_int __68;

    envs = (new set<str *>());

    FOR_IN(arg,arglist,66,68,69)
        if ((!(arg)->__contains__(const_21))) {
            break;
        }
        envs->add((arg->partition(const_21))->__getfast__(0));
    END_FOR

    return envs;
}

list<str *> *EnvFilter::get_command(list<str *> *userargs, list<str *> *exec_dirs) {
    str *__77, *__78, *to_exec;

    to_exec = __OR(this->get_exec(exec_dirs), this->exec_path, 77);
    return ((new list<str *>(1,to_exec)))->__add__((this->exec_args(userargs))->__slice__(1, 1, 0, 0));
}

__ss_bool EnvFilter::match(list<str *> *userargs) {
    list<str *> *user_command;
    set<str *> *filter_envs, *user_envs;
    __ss_bool __72, __73, __74;

    if (__eq(userargs->__getfast__(0), const_20)) {
        userargs->pop(0);
    }
    if ((len(userargs)<len(this->args))) {
        return False;
    }
    user_envs = this->_extract_env(userargs);
    filter_envs = this->_extract_env(this->args);
    user_command = userargs->__slice__(3, len(user_envs), (len(user_envs)+1), 0);
    return __AND(CommandFilter::match(user_command), __AND(___bool((len(filter_envs)!=0)), ___bool(__eq(user_envs, filter_envs)), 73), 72);
}

/**
class ChainingFilter
*/

class_ *cl_ChainingFilter;

__ss_bool ChainingFilter::is_chaining_filter() {
    
    return True;
}

/**
class IpNetnsExecFilter
*/

class_ *cl_IpNetnsExecFilter;

list<str *> *IpNetnsExecFilter::exec_args(list<str *> *userargs) {
    list<str *> *args;

    args = userargs->__slice__(1, 4, 0, 0);
    if (___bool(args)) {
        args->__setitem__(0, __os__::__path__::basename(args->__getfast__(0)));
    }
    return args;
}

__ss_bool IpNetnsExecFilter::match(list<str *> *userargs) {
    __ss_bool __86, __87;

    if ((__ne(this->run_as, const_2) or (len(userargs)<4))) {
        return False;
    }
    return ___bool(__eq(userargs->__slice__(2, 0, 3, 0), (new list<str *>(3,const_15,const_16,const_22))));
}

CommandFilter *build_filter(str *class_name, list<str *> *arg_list) {
    list<str *> *arguments;
    str *exe_name, *run_as;

    exe_name = arg_list->__getfast__(0);
    run_as = arg_list->__getfast__(1);
    arguments = arg_list->__slice__(1, 2, 0, 0);
    if (__eq(class_name, const_23)) {
        return (new CommandFilter(exe_name, run_as, arguments));
    }
    if (__eq(class_name, const_24)) {
        return ((CommandFilter *)((new RegExpFilter(exe_name, run_as, arguments))));
    }
    else if (__eq(class_name, const_25)) {
        return ((CommandFilter *)((new PathFilter(exe_name, run_as, arguments))));
    }
    else if (__eq(class_name, const_26)) {
        return ((CommandFilter *)((new KillFilter(run_as, arguments))));
    }
    else if (__eq(class_name, const_27)) {
        return ((CommandFilter *)((new ReadFileFilter(exe_name, arguments))));
    }
    else if (__eq(class_name, const_28)) {
        return ((CommandFilter *)((new IpFilter(exe_name, run_as, arguments))));
    }
    else if (__eq(class_name, const_29)) {
        return ((CommandFilter *)((new EnvFilter(exe_name, run_as, arguments))));
    }
    else if (__eq(class_name, const_30)) {
        return ((CommandFilter *)((new ChainingFilter(exe_name, run_as, arguments))));
    }
    else if (__eq(class_name, const_31)) {
        return ((CommandFilter *)((new IpNetnsExecFilter(exe_name, run_as, arguments))));
    }
    else {
        print2(NULL,0,1, __modct(const_32, 1, class_name));
        return NULL;
    }
    return 0;
}

void __init() {
    const_0 = new str("pass");
    const_1 = new str("");
    const_2 = new str("root");
    const_3 = new str("sudo");
    const_4 = new str("-u");
    const_5 = __char_cache[36];;
    const_6 = new str("/bin/kill");
    const_7 = new str("kill");
    const_8 = new str("/proc/%d/exe");
    const_9 = __char_cache[0];;
    const_10 = new str(" (deleted)");
    const_11 = new str("PATH");
    const_12 = __char_cache[58];;
    const_13 = new str("/bin/cat");
    const_14 = new str("cat");
    const_15 = new str("ip");
    const_16 = new str("netns");
    const_17 = new str("list");
    const_18 = new str("add");
    const_19 = new str("delete");
    const_20 = new str("env");
    const_21 = __char_cache[61];;
    const_22 = new str("exec");
    const_23 = new str("CommandFilter");
    const_24 = new str("RegExpFilter");
    const_25 = new str("PathFilter");
    const_26 = new str("KillFilter");
    const_27 = new str("ReadFileFilter");
    const_28 = new str("IpFilter");
    const_29 = new str("EnvFilter");
    const_30 = new str("ChainingFilter");
    const_31 = new str("IpNetnsExecFilter");
    const_32 = new str("Skipping unknown filter class (%s) specified in filter definitions");

    __name__ = new str("filters");

    default_0 = (new list<str *>());
    default_1 = (new list<str *>());
    cl_CommandFilter = new class_("CommandFilter");
    cl_RegExpFilter = new class_("RegExpFilter");
    default_2 = (new list<str *>());
    cl_PathFilter = new class_("PathFilter");
    cl_KillFilter = new class_("KillFilter");
    cl_ReadFileFilter = new class_("ReadFileFilter");
    cl_IpFilter = new class_("IpFilter");
    default_3 = (new list<str *>());
    cl_EnvFilter = new class_("EnvFilter");
    cl_ChainingFilter = new class_("ChainingFilter");
    cl_IpNetnsExecFilter = new class_("IpNetnsExecFilter");
}

} // module namespace

