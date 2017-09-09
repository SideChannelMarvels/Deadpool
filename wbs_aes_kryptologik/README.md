# KryptoPlus™ White-box challenge

Mehdi Sotoodeh published a challenge at http://kryptologik.com/ :

*You are invited to have a look at the code and evaluate it for yourself. Can you break it?*

The challenge whitebox [DemoKey_encrypt.js](http://kryptologik.com/js/DemoKey_encrypt.js) is written in JavaScript.

Some details are also available at https://github.com/msotoodeh/WhiteBoxCrypto.
There, it is said that KryptoPlus tools generates code for C/C++, JavaScript, Python and C-sharp.

Indeed the code is only using very simple operations and, besides some IO and initialisations, can almost be copy-pasted as such.
Therefore versions written in Python and in C are available in the `target` directory, using a file to store the tables (so it's a bit more comfortable to use from the command line and to attack by DFA).

Components
----------

* `target` contains the original challenge in JS and ports in C and Python, and instructions how to use it, as well as the solution (the key to recover).
* `traces` contains explanations on how to acquire a visual trace and snapshots.
* `DFA` contains the DFA attack against this challenge.

Other write-ups
---------------

