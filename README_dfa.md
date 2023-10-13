# Differential Fault Analysis

The DFA attacks are leveraged by two components:

 * this Deadpool project which provides
   * the ```deadpool_dfa.py``` script, see below;
   * several examples of applied DFA attacks agains white-box implementations.
 * [the JeanGrey project](https://github.com/SideChannelMarvels/JeanGrey), able to perform DFA attacks against faulty outputs.

## ```deadpool_dfa.py```

```deadpool_dfa.py``` is a Python 3 library to help acquiring faulty outputs.  
The current ```deadpool_dfa``` injects faults statically into a data file or an executable before execution. If there are integrity checks on the data or on the executable it should fail and a dynamic fault injection is required. That could be a future step.  
The script will take care of abnormal situations due to fault injection such as crashes or (presumably) infinite loops.

### Inputs and outputs

To interface with the white-box implementation under attack, you must define two helper functions in a very similar way to ```deadpool_dca.py```:  

```processinput()``` will take the input block defined as ```int iblock``` and the blocksize as ```int blocksize``` and will return a tuple (```bytes```, ```list of str```) that will be used in a [Popen](https://docs.python.org/2/library/subprocess.html) interface respectively for the command stdin and command arguments.

The most elementary one is the one defined by default, providing the input as a hex string argument, stdin being unused:
```python
def processinput(iblock, blocksize):
    return (None, ['%0*x' % (2*blocksize, iblock)])
```

An example to provide input as raw chars on stdin is:
```python
def processinput(iblock, blocksize):
    return (bytes.fromhex('%0*x' % (2*blocksize, iblock)), None)
```

If the setup is such that there is no input to provide for the Popen call, e.g. because processinput will write the input in a file or because the white-box implementation generates its own random input, processinput must return ```(None, None)```.

But in the case of a white-box implementation expecting an input file, consider using Bash process substitution such as e.g. ```<(echo 0001020304050607|xxd -r -p)```, which is made possible with Tracer option ```shell=True```, see below.

Besides providing the input data to the white-box implementation, processinput is responsible to provide it all the required options.

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

```Acquisition``` ```__init__``` arguments are:
  * ```targetbin```: (str) the executable, required. Must be in the PATH so prepend './' if needed.
  * ```targetdata```: (str) the file to be faulted, can be the tables file loaded by the white-box executable or the executable itself. It's not supposed to be provided, it'll be copied from ```goldendata```, therefore any existing ```targetdata``` will be destroyed!
  * ```goldendata```: (str) the original copy of the file to be faulted, ```targetdata``` faulty copies will be made during the attack. Must be different from ```targetdata```!
  * ```dfa```: DFA module, e.g. phoenixAES from JeanGrey, see below for module requirements
  * ```iblock```: (int) reference input block to provide to the executable
  Default: 0x74657374746573747465737474657374
  * ```processinput```: the helper function to prepare the input from ```iblock```, cf above.  
  Default: a helper writing the input in hex
  * ```processoutput```: the helper function to extract the output data, cf above.  
  Default: a helper expecting the output in hex
  * ```verbose```: (int) verbosity level  
  Default: 1
  * ```maxleaf```: (int) max size of faulty blocks, can be large when attacking raw tables, smaller when attacking serialized tables or executables as a large fault has very little chance to succeed  
  Default: 256*256
  * ```minleaf```: (int) min size of faulty blocks in the search phase. Same as above. The smaller, the longer the scan may take.  
  Default: 64
  * ```minleafnail```: (int) once an exploitable output is found in the scan phase, reduce the fault up to this size, in order to avoid multiple faults at once. Reduce if DFA tool fails on the recorded traces.  
  Default: 8
  * ```addresses```: (tuple) address range within ```goldendata``` where to inject faults; or (str) '/path/to/logfile' to replay address ranges specified in this log file, see below  
  Default: None => the entire address range of ```goldendata```
  * ```start_from_left```: (bool) scan should start from left? Else from right. Note that DFA attacks one of the last rounds so it may be faster starting from the right.
  Default: True
  * ```depth_first_traversal```: (bool) scan should dig from ```maxleaf``` to ```minleaf``` elements before getting to the next ```maxleaf``` segment? Else try all ```maxxleaf``` segments before going one level down
  Default: False
  * ```faults```: (int) once a ```minleafnail``` segment gives a potentially exploitable output, how many faults to try on the same spot (with other random values)?
  Default: 4
  * ```minfaultspercol```: (int) potentially exploitable outputs may be sorted in classes, how many faults to record a minima in each class before interrupting the attack? E.g. for AES the minimum is 2. If you don't want to interrupt before end of the scan, set it to ```None```
  Default: 4
  * ```timeoutfactor```: (int or float) to detect potentially infinite loops, the script measures the process time under normal conditions and interrupts the faulted process after ```timeoutfactor``` times the normal processing time.
  Default: 2
  * ```savetraces_format```: (str) ```'default'``` will save inputs and faulty outputs in a very basic format, suitable for JeanGrey. ```'trs'``` will save them in a format compatible with Riscure Inspector.
  Default: ```'default'```
  * ```logfile```: (str) 
  Default: None
  * ```tolerate_error```: (bool) 
  Default: False
  * ```encrypt```: (bool or None) specify if the target is implementing an encryption or a decryption. If left undefined, it will be detected. When attacking previous rounds, it has to be defined.
  Default: None
  * ```outputbeforelastrounds``` (bool) when attacking previous rounds, indicate what kind of output to record: the real output or the virtual output once the known last rounds are removed.
  Default: False
  * ```shell```: (bool) 
  Default: False
  * ```debug```: (bool) 
  Default: False

*Note: it might be that some parts of the API are still too specific to AES and will be revised and moved to DFA modules once other ciphers are added...*

The DFA module is supposed to provide the following elements:
  * ```check((int)output, (bool/None)encrypt, (int)verbose, (bool)init)```: a function validating corrupted outputs and returning a tuple (FaultStatus, index)
  * ```blocksize```
  * ```FaultStatus```: an enumeration of possible status: Crash, Loop, NoFault, MinorFault, MajorFault, WrongFault, GoodEncFault, GoodDecFault

When an attack is running, a logfile records the faults leading to potentially exploitable outputs. This logfile can be provided for a new set of attacks via the ```addresses``` argument to replay an attack at the same addresses.

Default saved traces format is very basic: ```dfa_<<enc/dec>>>_<<begin_timestamp>>-<<end_timestamp>>>_<<number of records>>.txt``` containing on each line the reference input and the output as hex string.
First record is the one with the correct output, to be used as reference by the DFA tool.

If the attack is running for long and you want to try a DFA attack on intermediate results, send a SIGUSR1 and it will dump an intermediate tracefile.
You can interrupt the script with a SIGINT (ctrl-C), it will save the current tracefile as well before quitting.

Once a tracer is instanciated, call ```run``` without arguments. It will return True if scan is interrupted because ```minfaultspercol``` condition is fulfilled, else False.

Typical usage:

```python
import deadpool_dfa
import phoenixAES
engine=deadpool_dfa.Acquisition(targetbin='./whitebox', targetdata='whitebox-tables.bin', goldendata='whitebox-tables.bin.gold', dfa=phoenixAES)
tracefiles=engine.run()
for tracefile in tracefiles:
    if phoenixAES.crack(tracefile):
        break
```

### DFA Usage Examples

* [DFA on Karroumi 2010](wbs_aes_karroumi2010/DFA)
* [DFA on NoSuchCon 2013](wbs_aes_nsc2013/DFA) with darkphoenixAES
* [DFA on NoSuchCon 2013 variant](wbs_aes_nsc2013_variants/DFA) and [on its sources](wbs_aes_nsc2013_variants/DFA_src)
* [DFA on PlaidCTF 2013](wbs_aes_plaidctf2013/DFA), variant [using Frida](wbs_aes_plaidctf2013/DFA2) and [PoC on round 8](wbs_aes_plaidctf2013/DFAr8)
* [DFA on CHES 2016](wbs_aes_ches2016/DFA)
* [DFA on SECCON 2016](wbs_aes_seccon2016/DFA)
* [DFA on Kryptologik](wbs_aes_kryptologik/DFA) recursing on all rounds
* [DFA on OpenWhiteBox AES Chow](wbs_aes_openwhitebox_chow/DFA)
* [DFA on OpenWhiteBox AES Xiao-Lai](wbs_aes_openwhitebox_xiaolai/DFA) with darkphoenixAES
* [DFA on Lee CASE1](wbs_aes_lee_case1/DFA)
* [DFA on RHME3 prequal](wbs_aes_rhme3_prequal/DFA)
* [DFA on GreHack 2019](wbs_aes_grehack2019/DFAr8) on round 8
