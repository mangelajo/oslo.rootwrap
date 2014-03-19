#include "builtin.hpp"
#include "os/path.hpp"
#include "signal.hpp"
#include "stat.hpp"
#include "sys.hpp"
#include "string.hpp"
#include "os/__init__.hpp"
#include "ConfigParser.hpp"
#include "re.hpp"
#include "wrapper.hpp"
#include "filters.hpp"
#include "cmd.hpp"

/**
Root wrapper for OpenStack services

Filters which commands a service is allowed to run as another user.

To use this with oslo, you should set the following in
oslo.conf:
rootwrap_config=/etc/oslo/rootwrap.conf

You also need to let the oslo user run oslo-rootwrap
as root in sudoers:
oslo ALL = (root) NOPASSWD: /usr/bin/oslo-rootwrap
                                /etc/oslo/rootwrap.conf *

Service packaging should deploy .filters files only on nodes where
they are needed, to avoid allowing more than is necessary.
*/

namespace __cmd__ {

str *const_0, *const_1, *const_2, *const_3, *const_4, *const_5, *const_6, *const_7, *const_8;


str *__name__;
__ss_int RC_BADCONFIG, RC_NOCOMMAND, RC_NOEXECFOUND, RC_UNAUTHORIZED;


__ss_bool  default_0;


void *_exit_error(str *execname, str *message, __ss_int errorcode, __ss_bool log) {
    
    print(1, __sys__::__ss_stderr, 0, 0, __modct(const_0, 2, execname, message));
    if (log) {
        print(1, 0, 0, 0, message);
    }
    __sys__::__ss_exit(errorcode);
    return NULL;
}

void *__ss_main() {
    list<str *> *command, *userargs;
    __wrapper__::RootwrapConfig *config;
    __ConfigParser__::RawConfigParser *rawconfig;
    str *configfile, *execname, *msg;
    __filters__::CommandFilter *filtermatch;
    __ss_int return_code;
    list<__filters__::CommandFilter *> *filters;

    execname = (__sys__::argv)->pop(0);
    if ((len(__sys__::argv)<2)) {
        _exit_error(execname, const_1, RC_NOCOMMAND, False);
    }
    configfile = (__sys__::argv)->pop(0);
    userargs = (__sys__::argv)->__slice__(0, 0, 0, 0);
    try {
        rawconfig = (new __ConfigParser__::RawConfigParser(NULL));
        rawconfig->read(configfile);
        config = (new __wrapper__::RootwrapConfig(rawconfig));
    } catch (ValueError *exc) {
        msg = __modct(const_2, 2, configfile, exc->message);
        _exit_error(execname, msg, RC_BADCONFIG, False);
    } catch (__ConfigParser__::Error *) {
        _exit_error(execname, __modct(const_3, 1, configfile), RC_BADCONFIG, False);
    }
    if (config->use_syslog) {
        __wrapper__::setup_syslog(execname, config->syslog_log_facility, config->syslog_log_level);
    }
    filters = __wrapper__::load_filters(config->filters_path);
    try {
        filtermatch = __wrapper__::match_filter(filters, userargs, config->exec_dirs);
        if (___bool(filtermatch)) {
            command = filtermatch->get_command(userargs, config->exec_dirs);
            if (config->use_syslog) {
                print(1, 0, 0, 0, const_4);
            }
            return_code = __os__::system((const_5)->join(command));
            __sys__::__ss_exit(return_code);
        }
    } catch (__wrapper__::FilterMatchNotExecutable *exc) {
        msg = __modct(const_6, 2, (exc->match)->exec_path, (exc->match)->name);
        _exit_error(execname, msg, RC_NOEXECFOUND, config->use_syslog);
    } catch (__wrapper__::NoFilterMatched *) {
        msg = __modct(const_7, 1, (const_5)->join(userargs));
        _exit_error(execname, msg, RC_UNAUTHORIZED, config->use_syslog);
    }
    return NULL;
}

void __init() {
    const_0 = new str("%s: %s");
    const_1 = new str("No command specified");
    const_2 = new str("Incorrect value in %s: %s");
    const_3 = new str("Incorrect configuration file: %s");
    const_4 = new str("logging.info omited");
    const_5 = __char_cache[32];;
    const_6 = new str("Executable not found: %s (filter match = %s)");
    const_7 = new str("Unauthorized command: %s (no filter matched)");
    const_8 = new str("__main__");

    __name__ = new str("__main__");

    RC_UNAUTHORIZED = 99;
    RC_NOCOMMAND = 98;
    RC_BADCONFIG = 97;
    RC_NOEXECFOUND = 96;
    default_0 = True;
    if (__eq(__name__, const_8)) {
        __ss_main();
    }
}

} // module namespace

int main(int __ss_argc, char **__ss_argv) {
    __shedskin__::__init();
    __stat__::__init();
    __os__::__path__::__init();
    __os__::__init();
    __signal__::__init();
    __sys__::__init(__ss_argc, __ss_argv);
    __string__::__init();
    __re__::__init();
    __ConfigParser__::__init();
    __filters__::__init();
    __wrapper__::__init();
    __shedskin__::__start(__cmd__::__init);
}
