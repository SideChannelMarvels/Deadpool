Usage
-----

Faulting multiple times the binary requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

It attacks the 8th round to break the last round key.  

Result is the last round key, so to roll back key scheduling up to initial AES key, one can e.g. use ```aes_keyschedule``` from Stark project.

```bash
aes_keyschedule D6737EF23E86AAF2A1C1F2952B499AA3 10
K00: 474831397B4145532069732046554E7D
```

When attacking all tables with range 0x15000-0x65000 (320kb), attack takes about 70s.  
When restricting to tables range 0x38000-0x40000 (32kb), attack takes about 7s.
