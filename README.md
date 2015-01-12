# lpt_reg
Simple parallel pord logger

lpr_timed - command-line LPT logger. Typical usage:
```
lpr_timed -o /mnt/ramdisk/test.bin -t 5
```

pybingraph.py - reads test.bin file created by lpr_timed, shows graph and exports data to test.csv (plain text). Usage:
```
python pybingraph.py
```

Bits 6:3 of the status register are used by default:
10 	Ack 	In 	Status-6
12 	Paper-Out 	In 	Status-5
13 	Select 	In 	Status-4 	
15 	Error 	In 	Status-3
