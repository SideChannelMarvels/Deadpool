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

What to trace is expressed by a list of filters. A ```Filter``` class is provided to help building filters.

```python
f=Filter(keyword, modes, condition, extract, extract_fmt)
```
 * ```keyword```: string that resumes the functionality of the filter. It will be used to build the filenames of the traces.
 * ```modes```: list of strings indicating which actions to capture, reads (```'R'```) and/or writes (```'W'```)
 * ```condition```: a function taking as arguments 
   * ```stack_range``` (a list or tuple of two ints indicating the boundaries of what to be considered as the stack)
   * ```addr``` (the current memory access address)
   * ```size``` (size of current memory access data)
   * ```data``` (current memory access data)
   * return value: True if conditions are met to record information about current event, else False
 * ```extract```: a function taking as arguments
   * ```addr``` (the current memory access address)
   * ```size``` (size of current memory access data)
   * ```data``` (current memory access data)
   * return value: the element to be recorded
 * ```extract_fmt```: a [packing format string](https://docs.python.org/2/library/struct.html) for the element to be recorded

A few usual filters are already defined in ```deadpool_dca.py``` in the ```DefaultFilters``` class

```python
class DefaultFilters:
    # Bytes written on stack:
    stack_w1      =Filter('stack_w1', ['W'], lambda stack_range, addr, size, data: stack_range[0] <= addr <= stack_range[1] and size == 1, lambda addr, size, data: data, '<B')
    stack_w4      =Filter('stack_w4', ['W'], lambda stack_range, addr, size, data: stack_range[0] <= addr <= stack_range[1] and size == 4, lambda addr, size, data: data, '<I')
    # Low byte(s) address of data read from data segment:
    mem_addr1_rw1 =Filter('mem_addr1_rw1', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 1, lambda addr, size, data: addr & 0xFF, '<B')
    mem_addr1_rw4 =Filter('mem_addr1_rw4', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 4, lambda addr, size, data: addr & 0xFF, '<B')
    mem_addr2_rw1 =Filter('mem_addr2_rw1', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 1, lambda addr, size, data: addr & 0xFFFF, '<H')
    # Bytes read from data segment:
    mem_data_rw1  =Filter('mem_data_rw1', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 1, lambda addr, size, data: data, '<B')
    mem_data_rw4  =Filter('mem_data_rw4', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 4, lambda addr, size, data: data, '<I')
```

So one can use any of them as shortcuts, e.g.:

```python.
myfilters=[DefaultFilters.stack_w4, DefaultFilters.mem_data_rw4]
```

### Tracing

### Converting binary traces to formats suitable for CPA tools

#### ```bin2daredevil```

#### ```bin2trs```

### Misc helpers

#### ```sample2event```

TODO
