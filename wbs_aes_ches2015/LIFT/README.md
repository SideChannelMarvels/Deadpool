Code lifting
------------

```extract_rom_tables_validate.py``` is a script which extracts tables from the original ```rom.bin```
into a format identical to the [NoSuchCon white-box generator](../../wbs_aes_nsc2013_variants/target/README.md) because, well, that's the same kind of white-box structure...
See [this write-up](https://wiki.yobi.be/index.php/CHES2015_Writeup#Second_step_revisited) for details.

```ches_2015_whitebox.c``` is identical to [```nosuchcon_2013_whitebox_allenc.c```](../../wbs_aes_nsc2013_variants/target/nosuchcon_2013_whitebox_allenc.c).
