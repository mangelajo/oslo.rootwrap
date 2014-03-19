#include "builtin.hpp"
#include "os/path.hpp"
#include "stat.hpp"
#include "string.hpp"
#include "os/__init__.hpp"
#include "ConfigParser.hpp"
#include "re.hpp"
#include "wrapper.hpp"
#include "filters.hpp"

namespace __wrapper__ {

str *const_0, *const_1, *const_10, *const_11, *const_12, *const_13, *const_2, *const_3, *const_4, *const_5, *const_6, *const_7, *const_8, *const_9;


str *__name__;


__filters__::CommandFilter * default_0;
void * default_3;
list<str *> * default_1;
void * default_2;

static inline list<str *> *list_comp_0(str *value);
static inline list<__filters__::CommandFilter *> *list_comp_1(list<__filters__::CommandFilter *> *filter_list, __filters__::CommandFilter *f);
static inline __ss_bool __lambda0__(str *f);

static inline list<str *> *list_comp_0(str *value) {
    list<str *> *__167;
    list<str *>::for_in_loop __170;
    str *__172, *s;
    __iter<str *> *__168;
    __ss_int __169;
    void *__171;

    list<str *> *__ss_result = new list<str *>();

    __167 = value->split(const_0);
    __ss_result->resize(len(__167));
    FOR_IN(s,__167,167,169,170)
        __ss_result->units[__169] = __string__::strip(s);
    END_FOR

    return __ss_result;
}

static inline list<__filters__::CommandFilter *> *list_comp_1(list<__filters__::CommandFilter *> *filter_list, __filters__::CommandFilter *f) {
    list<__filters__::CommandFilter *> *__179;
    __ss_int __181;
    list<__filters__::CommandFilter *>::for_in_loop __182;
    __iter<__filters__::CommandFilter *> *__180;
    __filters__::CommandFilter *fltr;

    list<__filters__::CommandFilter *> *__ss_result = new list<__filters__::CommandFilter *>();

    FOR_IN(fltr,filter_list,179,181,182)
        if (non_chain_filter(fltr, f)) {
            __ss_result->append(fltr);
        }
    END_FOR

    return __ss_result;
}

static inline __ss_bool __lambda0__(str *f) {
    
    return __NOT(f->startswith(const_1));
}

/**
class NoFilterMatched
*/

class_ *cl_NoFilterMatched;

void NoFilterMatched::__static__() {
}

/**
class FilterMatchNotExecutable
*/

class_ *cl_FilterMatchNotExecutable;

void *FilterMatchNotExecutable::__init__(__filters__::CommandFilter *match) {
    
    this->match = match;
    return NULL;
}

/**
class RootwrapConfig
*/

class_ *cl_RootwrapConfig;

void *RootwrapConfig::__init__(__ConfigParser__::RawConfigParser *config) {
    str *v;

    this->filters_path = (config->get(const_2, const_3, __ConfigParser__::default_3, NULL))->split(const_0);
    if (config->has_option(const_2, const_4)) {
        this->exec_dirs = (config->get(const_2, const_4, __ConfigParser__::default_3, NULL))->split(const_0);
    }
    else {
        this->exec_dirs = (new list<str *>());
        if ((__os__::__ss_environ)->__contains__(const_5)) {
            this->exec_dirs = ((__os__::__ss_environ)->__getitem__(const_5))->split(const_6);
        }
    }
    if (config->has_option(const_2, const_7)) {
        v = config->get(const_2, const_7, __ConfigParser__::default_3, NULL);
        this->syslog_log_facility = const_8;
    }
    else {
    }
    if (config->has_option(const_2, const_9)) {
        v = config->get(const_2, const_9, __ConfigParser__::default_3, NULL);
        this->syslog_log_level = v;
    }
    else {
        this->syslog_log_level = const_10;
    }
    if (config->has_option(const_2, const_11)) {
        this->use_syslog = config->getboolean(const_2, const_11);
    }
    else {
        this->use_syslog = False;
    }
    if (config->has_option(const_2, const_12)) {
        this->use_syslog_rfc_format = config->getboolean(const_2, const_12);
    }
    else {
        this->use_syslog_rfc_format = False;
    }
    return NULL;
}

void *setup_syslog(str *execname, str *facility, str *level) {
    
    return NULL;
}

__filters__::CommandFilter *build_filter(str *class_name, list<str *> *arg_list) {
    /**
    Returns a filter object of class class_name.
    */
    
    return __filters__::build_filter(class_name, arg_list);
}

list<__filters__::CommandFilter *> *load_filters(list<str *> *filters_path) {
    /**
    Load filters from a list of directories.
    */
    list<str *> *__152, *__156, *filterdefinition;
    __iter<str *> *__153, *__157;
    tuple2<str *, str *> *__160;
    __ConfigParser__::RawConfigParser *__166, *filterconfig;
    list<tuple2<str *, str *> *>::for_in_loop __164;
    void *__165;
    list<tuple2<str *, str *> *> *__161;
    list<str *>::for_in_loop __155, __159;
    str *filterdir, *filterfile, *name, *value;
    __filters__::CommandFilter *newfilter;
    __iter<tuple2<str *, str *> *> *__162;
    __ss_int __154, __158, __163;
    list<__filters__::CommandFilter *> *filterlist;

    filterlist = (new list<__filters__::CommandFilter *>());

    FOR_IN(filterdir,filters_path,152,154,155)
        if (__NOT(__os__::__path__::isdir(filterdir))) {
            continue;
        }

        FOR_IN(filterfile,filter(__lambda0__, __os__::listdir(filterdir)),156,158,159)
            filterconfig = (new __ConfigParser__::RawConfigParser(NULL));
            filterconfig->read(__os__::__path__::join(2, filterdir, filterfile));

            FOR_IN(__160,filterconfig->items(const_13),161,163,164)
                __160 = __160;
                name = __160->__getfast__(0);
                value = __160->__getfast__(1);
                filterdefinition = list_comp_0(value);
                newfilter = build_filter(filterdefinition->__getfast__(0), filterdefinition->__slice__(1, 1, 0, 0));
                if ((newfilter==NULL)) {
                    continue;
                }
                newfilter->name = name;
                filterlist->append(newfilter);
            END_FOR

        END_FOR

    END_FOR

    return filterlist;
}

__ss_bool non_chain_filter(__filters__::CommandFilter *fltr, __filters__::CommandFilter *f) {
    __ss_bool __173, __174;

    return __AND(___bool(__eq(fltr->run_as, f->run_as)), __NOT(fltr->is_chaining_filter()), 173);
}

__filters__::CommandFilter *match_filter(list<__filters__::CommandFilter *> *filter_list, list<str *> *userargs, list<str *> *exec_dirs) {
    /**
    Checks user command and arguments through command filters.
    
    Returns the first matching filter.
    
    Raises NoFilterMatched if no filter matched.
    Raises FilterMatchNotExecutable if no executable was found for the
    best filter match.
    */
    list<str *> *args;
    list<__filters__::CommandFilter *> *__175, *leaf_filters;
    __ss_bool __183, __184;
    __iter<__filters__::CommandFilter *> *__176;
    __filters__::CommandFilter *f, *first_not_executable_filter;
    __ss_int __177;
    list<__filters__::CommandFilter *>::for_in_loop __178;

    first_not_executable_filter = NULL;

    FOR_IN(f,filter_list,175,177,178)
        if (f->match(userargs)) {
            if (f->is_chaining_filter()) {
                leaf_filters = list_comp_1(filter_list, f);
                args = f->exec_args(userargs);
                if ((__NOT(___bool(args)) or __NOT(___bool(match_filter(leaf_filters, args, exec_dirs))))) {
                    continue;
                }
            }
            if (__NOT(___bool(f->get_exec(exec_dirs)))) {
                if (__NOT(___bool(first_not_executable_filter))) {
                    first_not_executable_filter = f;
                }
                continue;
            }
            return f;
        }
    END_FOR

    if (___bool(first_not_executable_filter)) {
        throw ((new FilterMatchNotExecutable(first_not_executable_filter)));
    }
    throw ((new NoFilterMatched()));
    return 0;
}

void __init() {
    const_0 = __char_cache[44];;
    const_1 = __char_cache[46];;
    const_2 = new str("DEFAULT");
    const_3 = new str("filters_path");
    const_4 = new str("exec_dirs");
    const_5 = new str("PATH");
    const_6 = __char_cache[58];;
    const_7 = new str("syslog_log_facility");
    const_8 = new str("none");
    const_9 = new str("syslog_log_level");
    const_10 = new str("ERROR");
    const_11 = new str("use_syslog");
    const_12 = new str("use_syslog_rfc_format");
    const_13 = new str("Filters");

    __name__ = new str("wrapper");

    cl_NoFilterMatched = new class_("NoFilterMatched");
    NoFilterMatched::__static__();
    default_0 = NULL;
    cl_FilterMatchNotExecutable = new class_("FilterMatchNotExecutable");
    cl_RootwrapConfig = new class_("RootwrapConfig");
    default_1 = (new list<str *>());
}

} // module namespace

