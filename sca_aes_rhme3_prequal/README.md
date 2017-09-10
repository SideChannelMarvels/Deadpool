# RHME3 prequal challenge

This one is a bit of an exception as it's not a whitebox but a side-channel challenge, but we can solve it with the help of Daredevil.

One of the prequal challenges of the RHME3 CTF was a set of power traces.  
Ref: https://rhme.riscure.com/3/

Components
----------

* `target` contains the original challenge.
* `DPA` contains the DPA attack against this challenge.

Other write-ups
---------------

* using Jlsca
  * https://github.com/ResultsMayVary/ctf/tree/master/RHME3/tracing
  * https://github.com/x8-999-github/cw-projects-experiments/blob/master/tracing_the_traces/tracing_the_traces_jlsca.ipynb
  * https://gist.github.com/dqi/88c86e484fc9302694837810680d2023
* using Chipwhisperer
  * https://github.com/x8-999-github/cw-projects-experiments/blob/master/tracing_the_traces/tracing_the_traces_cw.ipynb
  * https://govanify.com/post/rhme3-quals-ctf/
* using no specific SCA tool
  * https://gist.github.com/DavidBuchanan314/c6d4c529e307277d13ce17bcc39726fc
  * https://github.com/pakesson/ctf/blob/master/RHme3/qualifiers/tracing_the_traces/python_sca_notebook.ipynb
* See also https://ctftime.org/task/4527
