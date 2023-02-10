BGE attack
----------

We'll perform a BGE attack on intermediate rounds 2, 3, 4 and 5 with the tool [BlueGalaxyEnergy](https://github.com/SideChannelMarvels/BlueGalaxyEnergy).

Installation:
```bash
pip install bluegalaxyenergy
```

The whitebox code [nosuchcon_2013_whitebox.py](nosuchcon_2013_whitebox.py) is similar to the one in [RE/result/](../RE/result/nosuchcon_2013_whitebox.py) but rewritten as a class compatible with BlueGalaxyEnergy.

This attack requires the tables extracted from the reverse-engineering step, so if you didn't do it yet:

```bash
cd ../RE
./extract.sh
cd -
```

Then:

```bash
./runme.py
```

It breaks round keys and reverts the AES key scheduling till

```key: 4e5343234f707069646123b8dce442d0```

We can see that the challenge key starts actually with:

```bash
echo -n "NSC#Oppida#"|xxd -p
4e5343234f707069646123
```
