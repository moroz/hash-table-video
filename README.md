# Hash Table in C Tutorial

See the [YouTube video](https://youtu.be/tzEamfEoVag) for explanation on what this is.
The implementation of the hash table data structure in this repository is inspired by [Chapter 20](https://craftinginterpreters.com/hash-tables.html) of the book _Crafting Interpreters_ by Robert Nystrom.

## Installing dependencies

In order to run the code in this repository, you're going to need a C compiler, Make, and [Criterion](https://criterion.readthedocs.io/). On Debian-based Linux distributions (including Ubuntu, Linux Mint, and the like), you can install all of the above using apt:

```
sudo apt install -y build-essential libcriterion-dev
```

On macOS, you can install Criterion using [Homebrew](http://brew.sh/):

```
brew install criterion
```

## Run the tests

```
make test
```
