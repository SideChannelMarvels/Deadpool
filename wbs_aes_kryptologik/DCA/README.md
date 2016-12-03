Usage
-----

Using Tracer to acquire execution traces, here recording the lowest byte of each address from which data is being read:

```bash
./trace_it.py
00000 30A29995E87DF4FE75B0B901A7C398B2 -> AF75FA270FC54BDC9F439693D76C120B
00001 542249B18F231C6A4F7C9A85FE9435C7 -> 3336DAA8E5BAB5929F8B068602E116DA
00002 FD7554B5B776EF036AF4B203B3B1B72B -> B58206116DA6E72E78BDAB52F46358DB
00003 2C3B3C1C1380ECB01E247921458C6707 -> A41E271F65F4F54CF24339153ECD8A8C
00004 29CD9149DE989436C77A7FB38B26418C -> 7E7DA399C52FA92024C10F56877D6F40
...
```

Executing the differential analysis on the converted traces:

```bash
daredevil -c mem_addr1_r1_200_18304.attack_sbox.config
daredevil -c mem_data_r1_200_18304.attack_sbox.config
daredevil -c mem_stack_w1_200_33544.attack_sbox.config
```

With 100-200 traces, the first round key is easily recovered, no matter if
it's from stack, data or addr1 traces.

The current framework does not allow to break further rounds, tbd. 

See DFA for a full attack.
