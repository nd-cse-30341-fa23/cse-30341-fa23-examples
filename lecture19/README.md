Based on: http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/

Build kernel module:

    $ make

Look at module information

    $ modinfo quote.ko

Load module 

    $ insmod quote.ko

List all modules

    $ lsmod

Remove module

    $ rmmod quote

View kernel messages:

    $ dmesg
