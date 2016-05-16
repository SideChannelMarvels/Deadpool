Usage
-----

Faulting multiple times the binary requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_plaidctf.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/drmless tmp
cd tmp
./attack_plaidctf.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

The attack starts by patching drmless to always return the decrypted data (with a default ```.drmlicense``` filled with zeroes).

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
