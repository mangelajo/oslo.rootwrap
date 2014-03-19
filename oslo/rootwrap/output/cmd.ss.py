# Copyright (c) 2011 OpenStack Foundation.
# All Rights Reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#    License for the specific language governing permissions and limitations
#    under the License.

"""Root wrapper for OpenStack services

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
"""

from __future__ import print_function

#import logging
import os
#import pwd
import signal
#import subprocess
import sys
import wrapper

#from six import moves
import ConfigParser 
RC_UNAUTHORIZED = 99                     # [int]
RC_NOCOMMAND = 98                        # [int]
RC_BADCONFIG = 97                        # [int]
RC_NOEXECFOUND = 96                      # [int]


def _subprocess_setup():
    # Python installs a SIGPIPE handler by default. This is usually not what
    # non-Python subprocesses expect.
    signal.signal(signal.SIGPIPE, signal.SIG_DFL) # []


def _exit_error(execname, message, errorcode, log=True): # execname: [str], message: [str], errorcode: [int], log: [bool]
    print("%s: %s" % (execname, message), file=sys.stderr) # []
    if log:                              # [bool]
        #logging.error(message)
	print(message)                          # [str]
    sys.exit(errorcode)                  # [None]


def _getlogin():
    try:
        return os.getlogin()             # [str]
    except OSError:
        return (os.getenv('USER') or     # [str]
                os.getenv('USERNAME') or # [str]
                os.getenv('LOGNAME'))    # [str]


def main():
    # Split arguments, require at least a command
    execname = sys.argv.pop(0)           # [str]
    if len(sys.argv) < 2:                # [list(str)]
        _exit_error(execname, "No command specified", RC_NOCOMMAND, log=False) # [bool]

    configfile = sys.argv.pop(0)         # [str]
    userargs = sys.argv[:]               # [list(str)]

    # Add ../ to sys.path to allow running from branch
    possible_topdir = os.path.normpath(os.path.join(os.path.abspath(execname), # [str]
                                                    os.pardir, os.pardir)) # [str]
    if os.path.exists(os.path.join(possible_topdir, "oslo", "__init__.py")): # [bool]
        sys.path = [possible_topdir] + sys.path # []

    # Load configuration
    try:
        rawconfig = ConfigParser.RawConfigParser() # [ConfigParser::RawConfigParser]
        rawconfig.read(configfile)       # [list(str)]
        config = wrapper.RootwrapConfig(rawconfig) # [wrapper::RootwrapConfig]
    except ValueError as exc:
        msg = "Incorrect value in %s: %s" % (configfile, exc.message) # [str]
        _exit_error(execname, msg, RC_BADCONFIG, log=False) # [bool]
    except ConfigParser.Error:
        _exit_error(execname, "Incorrect configuration file: %s" % configfile, # [str]
                    RC_BADCONFIG, log=False) # [bool]

    if config.use_syslog:                # [bool]
        wrapper.setup_syslog(execname,   # [None]
                             config.syslog_log_facility, # [str]
                             config.syslog_log_level) # [str]

    # Execute command if it matches any of the loaded filters
    filters = wrapper.load_filters(config.filters_path) # [list(filters::CommandFilter)]
    try:
        filtermatch = wrapper.match_filter(filters, userargs, # [filters::CommandFilter]
                                           exec_dirs=config.exec_dirs) # [list(str)]
        if filtermatch:                  # [filters::CommandFilter]
            command = filtermatch.get_command(userargs, # [list(str)]
                                              exec_dirs=config.exec_dirs) # [list(str)]
            if config.use_syslog:        # [bool]
                #logging.info("(%s > %s) Executing %s (filter match = %s)" % (
                #    _getlogin(), pwd.getpwuid(os.getuid())[0],
                #    command, filtermatch.name))
		print("logging.info omited")           # [str]

            #obj = subprocess.Popen(command,
            #                       stdin=sys.stdin,
            #                       stdout=sys.stdout,
            #                       stderr=sys.stderr,
            #                       preexec_fn=_subprocess_setup,
            #                       env=filtermatch.get_environment(userargs))
            #obj.wait()
            #sys.exit(obj.returncode)
	    return_code = os.system(command)    # [int]
            sys.exit(return_code)        # [None]
    except wrapper.FilterMatchNotExecutable as exc:
        msg = ("Executable not found: %s (filter match = %s)" # [str]
               % (exc.match.exec_path, exc.match.name)) # []
        _exit_error(execname, msg, RC_NOEXECFOUND, log=config.use_syslog) # [bool]

    except wrapper.NoFilterMatched:
        msg = ("Unauthorized command: %s (no filter matched)" # [str]
               % ' '.join(userargs))     # [str]
        _exit_error(execname, msg, RC_UNAUTHORIZED, log=config.use_syslog) # [bool]

if __name__ == "__main__":               # []
    main()
