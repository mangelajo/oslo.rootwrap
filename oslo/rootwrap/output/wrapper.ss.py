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

#import logging
#import logging.handlers
import os
import string

#from six import moves
import ConfigParser

#from oslo.rootwrap import filters
import filters

class NoFilterMatched(Exception):
    """This exception is raised when no filter matched."""
    pass


class FilterMatchNotExecutable(Exception):
    """Raised when a filter matched but no executable was found."""
    def __init__(self, match=None):      # self: [FilterMatchNotExecutable], match: [filters::CommandFilter]
        self.match = match               # [filters::CommandFilter]


class RootwrapConfig(object):

    def __init__(self, config):          # self: [RootwrapConfig], config: [ConfigParser::RawConfigParser]
        # filters_path
        self.filters_path = config.get("DEFAULT", "filters_path").split(",") # [list(str)]

        # exec_dirs
        if config.has_option("DEFAULT", "exec_dirs"): # [bool]
            self.exec_dirs = config.get("DEFAULT", "exec_dirs").split(",") # [list(str)]
        else:
            self.exec_dirs = []          # [list()]
            # Use system PATH if exec_dirs is not specified
            if "PATH" in os.environ:     # [bool]
                self.exec_dirs = os.environ['PATH'].split(':') # [list(str)]

        # syslog_log_facility
        if config.has_option("DEFAULT", "syslog_log_facility"): # [bool]
            v = config.get("DEFAULT", "syslog_log_facility") # [str]
	    self.syslog_log_facility = "none"   # [str]
            #facility_names = logging.handlers.SysLogHandler.facility_names
            #self.syslog_log_facility = getattr(logging.handlers.SysLogHandler,
            #                                   v, None)
            #if self.syslog_log_facility is None and v in facility_names:
            #    self.syslog_log_facility = facility_names.get(v)
            #if self.syslog_log_facility is None:
            #    raise ValueError('Unexpected syslog_log_facility: %s' % v)
        else:
            #default_facility = logging.handlers.SysLogHandler.LOG_SYSLOG
            #self.syslog_log_facility = default_facility
	    pass

        # syslog_log_level
        if config.has_option("DEFAULT", "syslog_log_level"): # [bool]
            v = config.get("DEFAULT", "syslog_log_level") # [str]
            #self.syslog_log_level = logging.getLevelName(v.upper())
            self.syslog_log_level = v    # [str]
            #if (self.syslog_log_level == "Level %s" % v.upper()):
            #    raise ValueError('Unexpected syslog_log_level: %s' % v)
        else:
            #self.syslog_log_level = logging.ERROR
            self.syslog_log_level = "ERROR" # [str]
	    pass
        # use_syslog
        if config.has_option("DEFAULT", "use_syslog"): # [bool]
            self.use_syslog = config.getboolean("DEFAULT", "use_syslog") # [bool]
        else:
            self.use_syslog = False      # [bool]

        # use_syslog_rfc_format
        if config.has_option("DEFAULT", "use_syslog_rfc_format"): # [bool]
            self.use_syslog_rfc_format = config.getboolean( # [bool]
                "DEFAULT", "use_syslog_rfc_format") # [str]
        else:
            self.use_syslog_rfc_format = False # [bool]


def setup_syslog(execname, facility, level): # execname: [str], facility: [str], level: [str]
    #rootwrap_logger = logging.getLogger()
    #rootwrap_logger.setLevel(level)
    #handler = logging.handlers.SysLogHandler(address='/dev/log',
    #                                         facility=facility)
    #handler.setFormatter(logging.Formatter(
    #                     os.path.basename(execname) + ': %(message)s'))
    #rootwrap_logger.addHandler(handler)
    pass

def build_filter(class_name, arg_list):  # class_name: [str], arg_list: [list(str)]
    """Returns a filter object of class class_name."""
    return filters.build_filter(class_name, arg_list) # [filters::CommandFilter]


def load_filters(filters_path):          # filters_path: [list(str)]
    """Load filters from a list of directories."""
    filterlist = []                      # [list(filters::CommandFilter)]
    for filterdir in filters_path:       # [__iter(str)]
        if not os.path.isdir(filterdir): # [bool]
            continue
        for filterfile in filter(lambda f: not f.startswith('.'), # [__iter(str)]
                                 os.listdir(filterdir)): # [list(str)]
            filterconfig = ConfigParser.RawConfigParser() # [ConfigParser::RawConfigParser]
            filterconfig.read(os.path.join(filterdir, filterfile)) # [list(str)]
            for (name, value) in filterconfig.items("Filters"): # [tuple(str)]
                filterdefinition = [string.strip(s) for s in value.split(',')] # [list(str)]
                newfilter = build_filter(filterdefinition[0], # [filters::CommandFilter]
                                         filterdefinition[1:]) # [list(str)]
                if newfilter is None:    # [None]
                    continue
                newfilter.name = name    # [str]
                filterlist.append(newfilter) # [None]
    return filterlist                    # [list(filters::CommandFilter)]

def non_chain_filter(fltr, f):           # fltr: [filters::CommandFilter], f: [filters::CommandFilter]
    return (fltr.run_as == f.run_as      # [bool]
            and not fltr.is_chaining_filter()) # [bool]

def match_filter(filter_list, userargs, exec_dirs=[]): # filter_list: [list(filters::CommandFilter)], userargs: [list(str)], exec_dirs: [list(str)]
    """Checks user command and arguments through command filters.

    Returns the first matching filter.

    Raises NoFilterMatched if no filter matched.
    Raises FilterMatchNotExecutable if no executable was found for the
    best filter match.
    """
    first_not_executable_filter = None   # [None]

    for f in filter_list:                # [__iter(filters::CommandFilter)]
        if f.match(userargs):            # [bool]
            if f.is_chaining_filter():   # [bool]
                # This command calls exec verify that remaining args
                # matches another filter.

                leaf_filters = [fltr for fltr in filter_list # [list(filters::CommandFilter)]
                                if non_chain_filter(fltr, f)] # [filters::CommandFilter]
                args = f.exec_args(userargs) # [list(str)]
                if (not args or not match_filter(leaf_filters, # [list(str)]
                                                 args, exec_dirs=exec_dirs)): # [list(str)]
                    continue

            # Try other filters if executable is absent
            if not f.get_exec(exec_dirs=exec_dirs): # [str]
                if not first_not_executable_filter: # [filters::CommandFilter]
                    first_not_executable_filter = f # [filters::CommandFilter]
                continue
            # Otherwise return matching filter for execution
            return f                     # [filters::CommandFilter]

    if first_not_executable_filter:      # [filters::CommandFilter]
        # A filter matched, but no executable was found for it
        raise FilterMatchNotExecutable(match=first_not_executable_filter) # [filters::CommandFilter]

    # No filter matched
    raise NoFilterMatched()
