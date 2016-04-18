# Deadpool
*Our hero took the name Deadpool during the time he was experimented on:  
The guards had a dead pool and gambled on which test subject would die next...  
By the way, in the comics, the white boxes are Deadpool's crazy alternative inner voice.*

Deadpool is a repository of various public white-box cryptographic implementations and their practical attacks.

## Differential Computation Analysis

The DCA attacks are leveraged by Tracer and Daredevil projects, also available at https://github.com/SideChannelMarvels/,
as outlined in the paper

Joppe W. Bos and Charles Hubain and Wil Michiels and Philippe Teuwen:  
Differential Computation Analysis: Hiding your White-Box Designs is Not Enough  
Cryptology ePrint Archive, Report 2015/753, IACR, 2015.  
http://eprint.iacr.org/2015/753.pdf

Attacks presented in the paper were performed partly with a proprietary tool, that we replaced by
an opensource variant (Daredevil) in order to have a completely opensource toolchain.

Deadpool contains a script to help acquiring traces for DCA: ```deadpool_dca.py```.  
See [deadpool_dca.py documentation here](deadpool_dca.md).

## Differential Fault Analysis

The DFA attacks are leveraged by JeanGrey project, also available at https://github.com/SideChannelMarvels/.

Deadpool contains a script to help acquiring traces for DFA: ```deadpool_dfa.py```.  
See [deadpool_dfa.py documentation here](deadpool_dfa.md).
