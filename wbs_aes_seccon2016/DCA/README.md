Usage
-----

```trace_it.py``` performs the steps to try the DCA attack.
It's using the address range and the stack range discovered in the visual traces.

So far we couldn't break this challenge by simple DCA, probably due to a large (8-bit) internal encoding masking the S-Boxes.
See DFA for a successful attack.

What we tried:
* 2000 traces, 1-byte stack writes, input
* 2000 traces, 1-byte address read, input
* 2000 traces, 1-byte data read, input
