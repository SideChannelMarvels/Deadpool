from __future__ import print_function
from random import randrange
import subprocess
import sys
from multiprocessing import Pool


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)




def ranpt():
    plaintext = []
    for _ in xrange(16):
        plaintext.append("{:02X}".format(randrange(0, 256)))
    return plaintext


def f(i):
    plainst = ranpt()
    out, cipher, empty = subprocess.check_output(["./tracetransitions", "".join(plainst)]).split("\n")

    out = bytearray(map(int, out.split()))
    eprint("trace {}".format(i))
    eprint("".join(plainst) + " -> " + cipher)
    eprint("trace length:" + str(len(out)) + "\n")

    plainst = bytearray(map(lambda x: int(x, 16), plainst))
    cipherout = bytearray()
    for i in range(0, 32, 2):
        cipherout.append(int(cipher[i: i+2], 16))

    return out, plainst, cipherout

if __name__ == "__main__":
    subprocess.check_call(["make", "tracetransitions"])
    number_of_traces = 50
    p = Pool(4)
    print("tracing...")
    traces = p.map(f, range(number_of_traces))

    with open("plaintext.trace", "wb") as pt:
        with open("ciphertext.trace", "wb") as ci:
            with open("tracefile.trace", "wb") as tr:
                for trace in traces:
                    tr.write(trace[0])
                    pt.write(trace[1])
                    ci.write(trace[2])
                    trace_len = len(trace[0])

    with open("attack.config", "w") as f:
        f.write("""[Traces]
files=1
trace_type=i
transpose=true
index=0
nsamples={0}
trace=tracefile.trace {1} {0}

[Guesses]
files=1
guess_type=u
transpose=true
guess=ciphertext.trace {1} 16

[General]
threads=4
order=1
return_type=double
window=0
algorithm=AES
position=LUT/AES_AFTER_SBOXINV
round=0
bytenum=all
bitnum=all
memory=8G
top=20""".format(trace_len, number_of_traces))
