Usage
-----

It's a Windows 32-bit graphical crackme performing an AES128 encryption over the user input.
If the output is equal to "hack.lu-2009-ctf", one gets a success message.

Here is one automated execution where the right input is provided.  
GUI automation is provided by [xdotool](http://www.semicomplete.com/projects/xdotool/).

```bash
wine32 crackme.exe &
sleep 1
xdotool - << EOF
search --sync --limit 1 --name Crackme
sleep 0.1
key Tab Tab
key 1 9 2 shift+E shift+F 9 shift+E 6 1 1 6 4 shift+B shift+D 2 8 9 shift+F 7 7 3 shift+E 6 shift+C 9 1 0 1 shift+B 8 9 shift+C
key Tab space
EOF
```

Solution
--------

```bash
echo 192EF9E61164BD289F773E6C9101B89C|xxd -r -p|openssl enc -e -aes-128-ecb -nopad -K 142bbe0e2d22e48097497d5fac5b5926
hack.lu-2009-ctf
```
