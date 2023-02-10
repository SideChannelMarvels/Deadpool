DarkPhoenixAES attack
---------------------

We'll perform a DFA attack on externally encoded outputs with the tool [DarkPhoenix](https://github.com/SideChannelMarvels/DarkPhoenix).

Installation:
```bash
pip install darkphoenixAES
```

We will compile the whitebox as a library
```bash
build_lib.sh
```

It generates a random key that we can see in `raw_key.txt`, for example
```Key: 005e23f89a72db8173ddd7c7bb49436c```

Then we try to break the whitebox

```bash
./runme.py
```

It breaks round keys and reverts the AES key scheduling till for example

```key: 005e23f89a72db8173ddd7c7bb49436c```

