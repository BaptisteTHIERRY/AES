# AES

This project of M1 cryptography consist in implementing the algorithm AES in C with at least the mode ECB to encrypt and decrypt file of any size with key of legnth 128 bits.
For now, the avaible mode are ECB,CBC and CFB. The program can encrypt and decrypt file of any size with a key of size 128,192 or 256 bits.

## Build 

To build this project :

```shell
$ make 
```

## Usage

**TO COMPLETE**

## Test 

To test the program in the default configuration and to compute execution time, run :

```shell
$ make test 
```

To test a MODE in particular :

```shell
$ make test_MODE
```

## Miscellaneous

To remove intermediate file of compilation

```shell
$ make clean
```

To remove everyhting except the source file

```shell
$ make remove
```

To get some help

```shell
$ make help
```