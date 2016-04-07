Usage
-----

Goal is to provide the right DES key in input:

```bash
python check.py fd4185ff66a94add && echo OK || echo FAIL
FAIL
```

```bash
python check.py fd4185ff66a94afd && echo OK || echo FAIL
OK
```
