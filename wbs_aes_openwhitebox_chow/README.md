# OpenWhiteBox AES Chow challenge

There is no official challenge so we made our own based on the implementation available from the OpenWhiteBox project.

Ref: https://github.com/OpenWhiteBox/AES

Components
----------

* `target` contains a compiled version, instructions how to compile by yourself, and instructions how to use it, as well as the solution (the key to recover).
* `traces` contains explanations on how to acquire a visual trace and snapshots used in the tutorial.
* `DCA` contains the DCA attack against this challenge.
* `DFA` contains the DFA attack against this challenge.

See the projet wiki for the corresponding tutorial.

Other write-ups
---------------

It was not a real challenge, but the OpenWhiteBox project has a cryptanalysis module for Chow:

* https://github.com/OpenWhiteBox/AES/tree/master/cryptanalysis/chow
