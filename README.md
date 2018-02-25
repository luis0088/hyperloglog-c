# HyperLogLog C 

**Compiling**

To compile use the makefile, it requires zmq lib to be installed and the folder obj must exists. This results in two files, zero and main, main is just an example of hll and zero is for the zeromq communication with memcached.

**Executing**

The main file can be excecuted simply by ./main.

The zero file accepts 2 arguments -b and -l.

- b=Number of bits to use for the buckets calculation.
- l=Object limit to calculate the unique objects.

When the object limit is reached it will create a text file in the folder Result which must be created previously.
