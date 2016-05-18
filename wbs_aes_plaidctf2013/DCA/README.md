Usage
-----

```trace_it.py``` performs the steps to try the DCA attack.
It's using the address range and the stack range discovered in the visual traces.

The attack starts by patching ```drmless``` to always return the decrypted data (with a default ```.drmlicense``` filled with zeroes).

```
.text:0804EC17 B8 00 00 00 00                    mov     eax, 0               => B8 01 00 00 00
.text:0804EC1C 8D 74 26 00                       lea     esi, [esi+0]
.text:0804EC20 EB 05                             jmp     short loc_804EC27
.text:0804EC22 B8 01 00 00 00                    mov     eax, 1
.text:0804EC27 83 C4 50                          add     esp, 50h
.text:0804EC2A 5B                                pop     n
.text:0804EC2B 5E                                pop     esi
.text:0804EC2C 5D                                pop     ebp
.text:0804EC2D C3                                retn
```

This is not strictly needed but it helps getting the output to try attacking the last round as well.

So far we couldn't break this challenge by simple DCA, probably due to a large (8-bit) internal encoding masking the S-Boxes.
See DFA for a successful attack.

What we tried:
* 12500 traces, 1-byte stack writes, input
* 12500 traces, 1-byte stack writes, output
* 12500 traces, 4-byte stack writes, input
* 12500 traces, 4-byte stack writes, output
