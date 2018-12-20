# 2FA BT Linux PAM client

This Linux PAM (abbrv. for **p**luggable **a**uthentication **m**odule) is part of my bachelor thesis (available [here](https://example.com")).

When in use this PAM may extend an authentication scheme by adding an addtional authentication factor ('something you have') by requesting a (previously configured) known Bluetooth device. The Bluetooth device must be proximate to the system running the [daemon](https://github.com/tozu/ba-2fa-daemon/) that queries for mentioned device. Depending on _security level_ further information, such as an One-time-password (OTP) or HMAC keyword must be submitted as well.

## Installation

### Requirements

Have the following packages installed (e.g. via aptitude):

    apt install gcc             // (or 'apt install build-essential' - GNU C Compiler)
    apt install libcurl3-dev    // (cURL with OpenSSL)

### Build PAM

  1. Clone repository

    git clone https://github.com/tozu/ba-2fa-linux-pam.git

  2. Change into directory

    user@system:~$ cd ba-2fa-linux-pam/

  3. Adjust installation path of PAM in makefile (optional)

    user@system:~$ nano makefile
    ...
    PAM_DST ?= /lib/security/pam-bt.so    (/lib/security/ is Debian default)

  4. Build PAM

    user@system:~$ make
    user@system:~$ make install

### Installation

In order to use the 2FA BT Linux PAM you have to modify the config file for the selected application in **/etc/pam.d/** (default location Debian-based distributions).

In the case of adding additional authentication factor to the Linux login scheme you have to edit the _common-auth_.

The general structure of PAM config files are the following:

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

The _module-path_ is the location of the PAM (debian-default: /lib/security/)

The neccessary _module-arguments_ of the 2FA Linux PAM client that have to be supplied are the following:

    {1} daemon-certificate
    {2} ip address : port
    {3} security level
    {4} HMAC keyword (optional)


A vaild configuration example for the 2FA BT Linux PAM is:

    (type)  (control-flag)  (module-path)               (module-arguments)
     auth    required       /lib/security/bt-pam.so     /cert/daemon-cert.pem 192.168.1.254:3456 2

The above depicted configuration would extend the **auth**entication scheme
of an application (depending on which PAM config file in _/etc/pam.d/_ it has been added), using the 2FA BT Linux PAM **bt-pam`.`so**, located at **/lib/security/**.

This means, for example in the case this line has been added to **common-auth**, the login of the OS system would only succeed when **1st** the correct Bluetooth device is present/proximate as well as **2nd** the correct One-time-password (OTP) is submitted (to the daemon). This action is **required**, otherwise access is not granted.
