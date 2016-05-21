Usage
-----

Generating a white-box:

```bash
../target/main --create-table tables_karroumi_extenc.tbl --extEnc=1
```

Using Tracer to acquire execution traces, here recording the lowest byte of each address from which data is being read:

```bash
./trace_it.py
00000 C3820513AD8BBFB02A00A77F3900A554 -> 74DA53CF16A102C0AB901661CD09B5E1
00001 82A0622A9E174D124E13954D6642C76E -> 44103C013C53B6CD0D0353306B02EC43
00002 2E6B25452DE5B8611EEB599A8254CA4F -> 30120D2202B574F5A278A842273F1952
00003 F9583795173C0A8D038EB03C49997B67 -> 74CD62A3FE142CD4EC5C641B5946819F
00004 6D8E31710D0973CCAECA2B9131318206 -> 92F2FEFD3A54938278333ED1C5F5B4A5
...
```

By default the script is using TracerPIN but if you want you can change it to use TracerGrind

Executing the differential analysis on the converted traces:

```bash
daredevil -c mem_addr1_rw1_2000_17352.attack_sbox.config
daredevil -c mem_addr1_rw1_2000_17352.attack_multinv.config
```
