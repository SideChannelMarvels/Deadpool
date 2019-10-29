Attack
------
The DCA attack described here was found by Aleksei Udovenko (aleksei@affine.group) and programmed by Frederik Andersen (s083908@student.dtu.dk).

The challenge calls 2542 functions multiple times in what it calls rounds. At the end of every function it decides which round should be called next, we can this a transition to a new round. This attack is based on the fact that some rounds always transition to exactly 2 different rounds, each with a ~50% chance. We can trace these transitions by writing a 0 or 1 depending on which is taken. These traces leak the key if we do DCA starting from the ciphertext. It even gives an exact match. Curiously this does not work if we attack the first round.  

Usage
-----
One command runs all the steps in the attack:
```bash
make attack
```

Since we attack the last round, we need to reverse the key schedule. This can be done using [Stark](https://github.com/SideChannelMarvels/Stark):
```bash
aes_keyschedule 3e4b16d37537f883bcfb8bbed372adcb 10
```


Individual Steps
----------------

First, we generate the executable that prints the round numbers
```bash
make printrounds
```

Print the rounds called with a random plaintext:
```bash
python2 printrounds.py
```

Analyze which rounds transitions to trace and generate c code to trace them
```bash
python2 analyse.py > ifs.c
```

Generate the executable to trace transitions:
```bash
make tracetransitions
```

Trace the transitions and generate a config file for daredevil. [Here](https://github.com/SideChannelMarvels/Daredevil/wiki) is a description of how to attack the last round of AES.
```bash
python2 trace_it.py
```

Run [Daredevil](https://github.com/SideChannelMarvels/Daredevil):
```bash
daredevil -c attack.config
```

Reverse the key schedule:
```bash
aes_keyschedule 3e4b16d37537f883bcfb8bbed372adcb 10
```

