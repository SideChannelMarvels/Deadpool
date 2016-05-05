# NoSuchCon 2013 variants

We've seen in [NoSuchCon 2013 section](../wbs_aes_nsc2013/README.md) that Eloi Vanderbéken published his generator
so this section contains some experiments which were not possible in the original NoSuchCon challenge.

Components
----------

* `target` contains the original white-box generator, variants, standalone white-box and instructions how to use them.
* `DCA` contains the DCA attack against the version without encodings.
* `DFA` contains the DFA attack against the (executable of the) version with encodings.
* `DFA_src` contains the DFA attack against the (source of the) version with encodings.
* `ALG` contains the algebraic attack used against the original challenge, here against the version with encodings.
* `ALG2` contains another algebraic attack against the version  with encodings, which is possible because the external encodings are available.
