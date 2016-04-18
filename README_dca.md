# Differential Computation Analysis

The DCA attacks are leveraged by three components:

 * [the Tracer project](https://github.com/SideChannelMarvels/Tracer), comprising several tracing modules.
 * this Deadpool project which provides
   * the ```deadpool_dca.py``` script, see below;
   * several examples of applied DCA attacks agains white-box implementations.
 * [the Daredevil project](https://github.com/SideChannelMarvels/Daredevil), able to perform CPA attacks against traces.

Our paper explains the principles behind the DCA attacks:

Joppe W. Bos and Charles Hubain and Wil Michiels and Philippe Teuwen:  
Differential Computation Analysis: Hiding your White-Box Designs is Not Enough  
Cryptology ePrint Archive, Report 2015/753, IACR, 2015.  
http://eprint.iacr.org/2015/753.pdf

Attacks presented in the paper were performed partly with a proprietary tool, that we now replaced by
[Daredevil](https://github.com/SideChannelMarvels/Daredevil) in order to have a completely opensource toolchain.

## ```deadpool_dca.py```

```deadpool_dca.py``` is a Python 2 library to help acquiring execution traces.  
It's using [Tracer](https://github.com/SideChannelMarvels/Tracer) tools to acquire traces that can be used with [Daredevil](https://github.com/SideChannelMarvels/Daredevil).

### Inputs and outputs

To interface with the white-box implementation under attack, you must define two helper functions:  

```processinput()``` will take the input block defined as ```int iblock``` and the blocksize as ```int blocksize``` and will return a ```list of str``` that will be used in a [Popen](https://docs.python.org/2/library/subprocess.html) interface.

The most elementary one is the one defined by default, returning the input as a hex string:
```python
def processinput(iblock, blocksize):
    return ['%0*x' % (2*blocksize, iblock)]
```

If the setup is such that there is no input to provide for the Popen call, e.g. because processinput will write the input in a file or because the white-box implementation generates its own random input, processinput must return ```None```.

But in the case of a white-box implementation expecting an input file, consider using Bash process substitution such as e.g. ```<(echo 0001020304050607|xxd -r -p)```, which is made possible with Tracer option ```shell=True```, see below.

Besides providing the input data to the white-box implementation, processoutput is responsible to provide it all the required options.

Each parameter must be provided in a separate string of the returned list. If Tracer option ```shell=True``` will be used, then it's not required to separate each parameter but the string must still be in a list.

Examples:
```python
return ['--mode', 'encryption', '--input', '%0*x' % (2*blocksize, iblock)]
```

Variant requiring ```shell=True```:
```python
return ['--mode encryption --input %0*x' % (2*blocksize, iblock)]
```

process substitution requires ```shell=True```:
```python
return ['--input-file <(echo %0*x|xxd -r -p) --output-file >(xxd -p)' % (2*blocksize, iblock)]
```

```processoutput()``` will take the output of the white-box implementation defined as a multiline ```str output``` and the blocksize as ```int blocksize``` and will return the output block as an ```int```.

The most elementary one is the one defined by default, expecting the white-box output to be a hex string:
```python
def processoutput(output, blocksize):
    return int(output, 16)
```

If the setup is such that there is no output to process, processoutput must return ```None```.

It might be that the output of the white-box is in a file to be read, in such case processoutput would ignore its output argument and open the file itself but in that situation, consider using Bash process substitution such as e.g. specifying an output file to the whitebox such as ```>(xxd -p)```, which is made possible with Tracer option ```shell=True```, see below.

Examples:

Expecting one of the output lines to be the hex dump of the output:
```python
return int(''.join([x for x in output.split('\n') if len(x)==32][0]), 16)
```

If no output available:
```python
return None
```

### Filters

### Tracing

### Converting binary traces to formats suitable for CPA tools

#### ```bin2daredevil```

#### ```bin2trs```

### Misc helpers

#### ```sample2event```

TODO
