Usage
-----

Here is a variant using Frida to call directly the inner white-box function.
Note that the fault injection is still done statically, not dynamically.

Faulting multiple times the binary requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_plaidctf.py spawn_drmless.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/drmless tmp/drmless.gold
cd tmp
./attack_plaidctf.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey)
as well as Frida to directly call the inner white-box function.

Nevertheless, because of Frida instrumentation, execution is much slower, even if we're only calling the white-box once per execution.
