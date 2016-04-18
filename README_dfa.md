# Differential Fault Analysis

The DFA attacks are leveraged by two components:

 * this Deadpool project which provides
   * the ```deadpool_dfa.py``` script, see below;
   * several examples of applied DFA attacks agains white-box implementations.
 * [the JeanGrey project](https://github.com/SideChannelMarvels/JeanGrey), able to perform DFA attacks against faulty outputs.

## ```deadpool_dfa.py```

```deadpool_dfa.py``` is a Python 3 library to help acquiring faulty outputs.

### Inputs and outputs

To interface with the white-box implementation under attack, you must define two helper functions in a very similar way to ```deadpool_dca.py```:  

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

It might be that the output of the white-box is in a file to be read, in such case processoutput would ignore its output argument and open the file itself but in that situation, consider using Bash process substitution such as e.g. specifying an output file to the whitebox such as ```>(xxd -p)```, which is made possible with Tracer option ```shell=True```, see below.

For a successful DFA we need to get the output under normal conditions, otherwise there is nothing to attack!
So under normal conditions processoutput is always expected to return an output.

Now as we're faulting the white-box implementation it might be that for some faults, the program does not output anything, or even crashes or even is in an infinite loop.
Therefore internally processoutput is wrapped in a try-catch that will output ```None``` in case of failure.

Examples:

Expecting one of the output lines to be the hex dump of the output:
```python
return int(''.join([x for x in output.split('\n') if len(x)==32][0]), 16)
```

### Acquisition

The current ```deadpool_dfa``` injects faults statically into a data file or an executable before execution. If there are integrity checks on the data or on the executable it should fail and a dynamic fault injection is required. That will be a future step.

TODO
