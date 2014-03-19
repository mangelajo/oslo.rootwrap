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

import os
import re


class CommandFilter(object):
    """Command filter only checking that the 1st argument matches exec_path."""

    def __init__(self, exec_path, run_as, args_list): # self: [CommandFilter], exec_path: [str], run_as: [str], args_list: [list(str)]
        self.name = ''                   # [str]
        self.exec_path = exec_path       # [str]
        self.run_as = run_as             # [str]
        self.args = args_list            # [list(str)]
        self.real_exec = None            # [None]

    def get_exec(self, exec_dirs=[]):    # self: [CommandFilter], exec_dirs: [list(str)]
        """Returns existing executable, or empty string if none found."""
        if self.real_exec is not None:   # [str]
            return self.real_exec        # [str]
        self.real_exec = ""              # [str]
        if os.path.isabs(self.exec_path): # [bool]
            if os.access(self.exec_path, os.X_OK): # [bool]
                self.real_exec = self.exec_path # [str]
        else:
            for binary_path in exec_dirs: # [__iter(str)]
                expanded_path = os.path.join(binary_path, self.exec_path) # [str]
                if os.access(expanded_path, os.X_OK): # [bool]
                    self.real_exec = expanded_path # [str]
                    break
        return self.real_exec            # [str]

    def match(self, userargs):           # self: [CommandFilter], userargs: [list(str)]
        """Only check that the first argument (command) matches exec_path."""
        return userargs and os.path.basename(self.exec_path) == userargs[0] # [bool]

    def get_command(self, userargs, exec_dirs=[]): # self: [CommandFilter], userargs: [list(str)], exec_dirs: [list(str)]
        """Returns command to execute (with sudo -u if run_as != root)."""
        to_exec = self.get_exec(exec_dirs=exec_dirs) or self.exec_path # [str]
        if (self.run_as != 'root'):      # []
            # Used to run commands at lesser privileges
            return ['sudo', '-u', self.run_as, to_exec] + userargs[1:] # [list(str)]
        return [to_exec] + userargs[1:]  # []

    def get_environment(self, userargs): # self: [], userargs: []
        """Returns specific environment to set, None if none."""
        return None                      # [None]

    def is_chaining_filter(self):        # self: [CommandFilter]
        return False                     # [bool]
    
    def exec_args(self, userargs):       # self: [CommandFilter], userargs: [list(str)]
        return []                        # []

class RegExpFilter(CommandFilter):
    """Command filter doing regexp matching for every argument."""

    def match(self, userargs):           # self: [RegExpFilter], userargs: [list(str)]
        # Early skip if command or number of args don't match
        if (not userargs or len(self.args) != len(userargs)): # []
            # DENY: argument numbers don't match
            return False                 # [bool]
        # Compare each arg (anchoring pattern explicitly at end of string)
        for (pattern, arg) in zip(self.args, userargs): # [tuple(str)]
            try:
                if not re.match(pattern + '$', arg): # [re::match_object]
                    break
            except re.error:
                # DENY: Badly-formed filter
                return False             # [bool]
        else:
            # ALLOW: All arguments matched
            return True                  # [bool]

        # DENY: Some arguments did not match
        return False                     # [bool]


class PathFilter(CommandFilter):         # [list(str)]
    """Command filter checking that path arguments are within given dirs

        One can specify the following constraints for command arguments:
            1) pass     - pass an argument as is to the resulting command
            2) some_str - check if an argument is equal to the given string
            3) abs path - check if a path argument is within the given base dir

        A typical rootwrapper filter entry looks like this:
            # cmdname: filter name, raw command, user, arg_i_constraint [, ...]
            chown: PathFilter, /bin/chown, root, nova, /var/lib/images

    """

    def match(self, userargs):           # self: [PathFilter], userargs: [list(str)]
        if not userargs or len(userargs) < 2: # [int]
            return False                 # [bool]

        command, arguments = userargs[0], userargs[1:] # [str], [list(str)]

        equal_args_num = len(self.args) == len(arguments) # [bool]
        exec_is_valid = super(PathFilter, self).match(userargs) # [bool]
        args_equal_or_pass = all(        # [bool]
            arg == 'pass' or arg == value # []
            for arg, value in zip(self.args, arguments) # [__iter(bool)]
            if not os.path.isabs(arg)    # [bool]
        )
        paths_are_within_base_dirs = all( # [bool]
            os.path.commonprefix([arg, os.path.realpath(value)]) == arg # []
            for arg, value in zip(self.args, arguments) # [tuple(str)]
            if os.path.isabs(arg)        # [bool]
        )

        return (equal_args_num and       # [bool]
                exec_is_valid and        # [bool]
                args_equal_or_pass and   # [bool]
                paths_are_within_base_dirs) # [bool]

    def get_command(self, userargs, exec_dirs=[]): # self: [PathFilter], userargs: [list(str)], exec_dirs: [list(str)]
        command, arguments = userargs[0], userargs[1:] # [str], [list(str)]

        # convert path values to canonical ones; copy other args as is
        args = [os.path.realpath(value) if os.path.isabs(arg) else value # [list(str)]
                for arg, value in zip(self.args, arguments)] # [list(str)]

        return super(PathFilter, self).get_command([command] + args, # [list(str)]
                                                   exec_dirs) # [list(str)]


class KillFilter(CommandFilter):         # [None]
    """Specific filter for the kill calls.

       1st argument is the user to run /bin/kill under
       2nd argument is the location of the affected executable
           if the argument is not absolute, it is checked against $PATH
       Subsequent arguments list the accepted signals (if any)

       This filter relies on /proc to accurately determine affected
       executable, so it will only work on procfs-capable systems (not OSX).
    """

    def __init__(self, run_as, args_list): # self: [KillFilter], run_as: [str], args_list: [list(str)]
        super(KillFilter, self).__init__("/bin/kill", run_as, args_list) # [list(str)]

    def match(self, userargs):           # self: [KillFilter], userargs: [list(str)]
        if not userargs or userargs[0] != "kill": # []
            return False                 # [bool]
        args = list(userargs)            # [list(str)]
        if len(args) == 3:               # []
            # A specific signal is requested
            signal = args.pop(1)         # [str]
            if signal not in self.args[1:]: # [bool]
                # Requested signal not in accepted list
                return False             # [bool]
        else:
            if len(args) != 2:           # []
                # Incorrect number of arguments
                return False             # [bool]
            if len(self.args) > 1:       # [list(str)]
                # No signal requested, but filter requires specific signal
                return False             # [bool]
        try:
            command = os.readlink("/proc/%d/exe" % int(args[1])) # [str]
        except (ValueError, OSError):
            # Incorrect PID
            return False                 # [bool]

        # NOTE(yufang521247): /proc/PID/exe may have '\0' on the
        # end, because python doesn't stop at '\0' when read the
        # target path.
        command = command.partition('\0')[0] # [str]

        # NOTE(dprince): /proc/PID/exe may have ' (deleted)' on
        # the end if an executable is updated or deleted
        if command.endswith(" (deleted)"): # [bool]
            command = command[:-len(" (deleted)")] # [str]

        kill_command = self.args[0]      # [str]

        if os.path.isabs(kill_command):  # [bool]
            return kill_command == command # [bool]

        return (os.path.isabs(command) and # [bool]
                kill_command == os.path.basename(command) and # []
                os.path.dirname(command) in os.environ.get('PATH', '' # [list(str)]
                                                           ).split(':')) # [bool]


class ReadFileFilter(CommandFilter):     # [None]
    """Specific filter for the utils.read_file_as_root call."""

    def __init__(self, file_path, args_list): # self: [ReadFileFilter], file_path: [str], args_list: [list(str)]
        self.file_path = file_path       # [str]
        super(ReadFileFilter, self).__init__("/bin/cat", "root", args_list) # [list(str)]

    def match(self, userargs):           # self: [ReadFileFilter], userargs: [list(str)]
        return (userargs == ['cat', self.file_path]) # [bool]


class IpFilter(CommandFilter):
    """Specific filter for the ip utility to that does not match exec."""

    def match(self, userargs):           # self: [IpFilter], userargs: [list(str)]
        if userargs[0] == 'ip':          # []
            if userargs[1] == 'netns':   # []
                return (userargs[2] in ('list', 'add', 'delete')) # [bool]
            else:
                return True              # [bool]


class EnvFilter(CommandFilter):          # [bool]
    """Specific filter for the env utility.

    Behaves like CommandFilter, except that it handles
    leading env A=B.. strings appropriately.
    """

    def _extract_env(self, arglist):     # self: [EnvFilter], arglist: [list(str)]
        """Extract all leading NAME=VALUE arguments from arglist."""

        envs = set()                     # [set(str)]
        for arg in arglist:              # [__iter(str)]
            if '=' not in arg:           # [bool]
                break
            envs.add(arg.partition('=')[0]) # [None]
        return envs                      # [set(str)]

    def __init__(self, exec_path, run_as, args_list): # self: [EnvFilter], exec_path: [str], run_as: [str], args_list: [list(str)]
        super(EnvFilter, self).__init__(exec_path, run_as, args_list) # [list(str)]

        env_list = self._extract_env(self.args) # [set(str)]
        # Set exec_path to X when args are in the form of
        # env A=a B=b C=c X Y Z
        if "env" in exec_path and len(env_list) < len(self.args): # [bool]
            self.exec_path = self.args[len(env_list)] # [str]

    def match(self, userargs):           # self: [EnvFilter], userargs: [list(str)]
        # ignore leading 'env'
        if userargs[0] == 'env':         # []
            userargs.pop(0)              # [str]

        # require one additional argument after configured ones
        if len(userargs) < len(self.args): # [list(str)]
            return False                 # [bool]

        # extract all env args
        user_envs = self._extract_env(userargs) # [set(str)]
        filter_envs = self._extract_env(self.args) # [set(str)]
        user_command = userargs[len(user_envs):len(user_envs) + 1] # [list(str)]

        # match first non-env argument with CommandFilter
        return (super(EnvFilter, self).match(user_command) # [bool]
                and len(filter_envs)!=0 and user_envs == filter_envs) # []

    def exec_args(self, userargs):       # self: [EnvFilter], userargs: [list(str)]
        args = userargs[:]               # [list(str)]

        # ignore leading 'env'
        if args[0] == 'env':             # []
            args.pop(0)                  # [str]

        # Throw away leading NAME=VALUE arguments
        while args and '=' in args[0]:   # [bool]
            args.pop(0)                  # [str]

        return args                      # [list(str)]

    def get_command(self, userargs, exec_dirs=[]): # self: [EnvFilter], userargs: [list(str)], exec_dirs: [list(str)]
        to_exec = self.get_exec(exec_dirs=exec_dirs) or self.exec_path # [str]
        return [to_exec] + self.exec_args(userargs)[1:] # [list(str)]

    def get_environment(self, userargs): # self: [], userargs: []
        env = os.environ.copy()          # [dict(str, str)]

        # ignore leading 'env'
        if userargs[0] == 'env':         # []
            userargs.pop(0)              # []

        # Handle leading NAME=VALUE pairs
        for a in userargs:               # []
            env_name, equals, env_value = a.partition('=') # []
            if not equals:               # []
                break
            if env_name and env_value:   # []
                env[env_name] = env_value # []

        return env                       # [dict(str, str)]


class ChainingFilter(CommandFilter):

    def is_chaining_filter(self):        # self: [ChainingFilter]
        return True                      # [bool]

class IpNetnsExecFilter(ChainingFilter):
    """Specific filter for the ip utility to that does match exec."""

    def match(self, userargs):           # self: [IpNetnsExecFilter], userargs: [list(str)]
        # Network namespaces currently require root
        # require <ns> argument
        if self.run_as != "root" or len(userargs) < 4: # []
            return False                 # [bool]

        return (userargs[:3] == ['ip', 'netns', 'exec']) # [bool]

    def exec_args(self, userargs):       # self: [IpNetnsExecFilter], userargs: [list(str)]
        args = userargs[4:]              # [list(str)]
        if args:                         # [list(str)]
            args[0] = os.path.basename(args[0]) # [str]
        return args                      # [list(str)]


def build_filter(class_name, arg_list):  # class_name: [str], arg_list: [list(str)]
    exe_name = arg_list[0]               # [str]
    run_as = arg_list[1]                 # [str]
    arguments = arg_list[2:]             # [list(str)]
    if class_name is "RegExpFilter":     # [str]
	return RegExpFilter(exe_name, run_as, arguments) # [RegExpFilter]
    elif class_name is "PathFilter":     # [str]
        return PathFilter(exe_name, run_as, arguments) # [PathFilter]
    elif class_name is "KillFilter":     # [str]
        return KillFilter(run_as, arguments) # [KillFilter]
    elif class_name is "ReadFileFilter": # [str]
        return ReadFileFilter(exe_name, arguments) # [ReadFileFilter]
    elif class_name is "IpFilter":       # [str]
        return IpFilter(exe_name, run_as, arguments) # [IpFilter]
    elif class_name is "EnvFilter":      # [str]
        return EnvFilter(exe_name, run_as, arguments) # [EnvFilter]
    elif class_name is "ChainingFilter": # [str]
        return ChainingFilter(exe_name, run_as, arguments) # [ChainingFilter]
    elif class_name is "IpNetnsExecFilter": # [str]
        return IpNetnsExecFilter(exe_name, run_as, arguments) # [IpNetnsExecFilter]
    else: 
        print("Skipping unknown filter class (%s) specified " # [str]
              "in filter definitions" % class_name) # []
        return None                      # [None]


def _shedskin_arg_discovery_helpers():
    build_filter(["ping","root","127.0.0.1","-t","2"]) # []
