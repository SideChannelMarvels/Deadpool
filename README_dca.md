# Differential Computation Analysis

The DCA attacks are leveraged by three components:

 * [the Tracer project](https://github.com/SideChannelMarvels/Tracer), comprising several tracing modules.
 * this Deadpool project which provides
   * the ```deadpool_dca.py``` script, see below;
   * several examples of applied DCA attacks agains white-box implementations.
 * [the Daredevil project](https://github.com/SideChannelMarvels/Daredevil), able to perform CPA attacks against traces.

Our paper explains the principles behind the DCA attacks:

Joppe W. Bos and Charles Hubain and Wil Michiels and Philippe Teuwen:  
Differential Computation Analysis: Hiding your White-Box Designs is Not Enough  
Cryptology ePrint Archive, Report 2015/753, IACR, 2015.  
http://eprint.iacr.org/2015/753.pdf

Attacks presented in the paper were performed partly with a proprietary tool, that we now replaced by
[Daredevil](https://github.com/SideChannelMarvels/Daredevil) in order to have a completely opensource toolchain.

# ```deadpool_dca.py```

```deadpool_dca.py``` is a Python 2 library to help acquiring execution traces.

TODO
