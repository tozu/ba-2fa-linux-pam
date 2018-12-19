# 2FA Linux PAM client

This Linux PAM (abbrv. for '**p**luggable **a**uthentication **m**odule') is part of my bachelor thesis (available [here](https://example.com")).

[TODO]

## Installation

### Requirements

[TODO]

### Installation

use PAM for other applications:
  modify config file for selected application in **/etc/pam.d/**.

In the case of adding authentication factor to the Linux login scheme you have to edit the _common-auth_.

The structure of PAM config files are the following:

    type      control-flag      module-path     module-arguments

These are the available PAM _types_:

    type            definition
    --------------------------------------------------------------------------------
    auth            Module determines if the user is allowed to access the service,
                    if their passwords has expired, etc. E.g. requests or validates
                    a password.
                    
    account         Module verifies that the acces of the user is allowed and who
                    they claim to be, by a password or using biometrics.
    
    session         Module configures and manages user sessions, but it can also
                    perform additional tasks that are needed to allow access.
    
    password        Module is used to change user passwords.


These are the available _control-flag_'s:

    control-flag    definition
    --------------------------------------------------------------------------------
    required        If the module fails, PAM immediately returns a faillure result
                    to to the application; no further modules in the stack are
                    called.
                    
    requisite       If the module fails, PAM returns a failure result to the appli-
                    cation but it will continue to call the next module in the
                    stack.

    sufficient      If the module succeeds, PAM returns a 'pass' result to the ap-
                    plication and no futher modules in the stack are called.

    optional        The pass/fail result of the module is ignored, which generally
                    means that the module is being called to perform some operation,
                    rather than participating in the pass/fail decision for the
                    stack.

The _module-path_ of this (any other) PAM locatin is located at: {...path...}

The neccessary _module-arguments_ of the 2FA Linux PAM client that have to be supplied are the following:

    {1} daemon-certificate
    {2} ip address : port
    {3} security level
    {4} HMAC keyword (optional)

## TODO:

- [ ] finish writing description
- [ ] insert required packages
- [ ] complete installation section
