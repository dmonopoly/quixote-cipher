## What is this?
An implementation of the EM-based decipherment technique in [Knight and Yamada,
99](http://www.aclweb.org/anthology/W/W99/W99-0906.pdf) on Spanish sounds, given
separate IPA training material that is not parallel to historical text T (Don
Quixote, here).

### Purpose
Research. 

### Code Organization
TODO

## Running (not ready yet)
Compile using cmake. Example:

    mkdir build && cd build
    cmake ..
    make

Run the generated executable. Alternatively run any test executables, but make
sure you do `cmake -Dtest=ON ..` first.

Running Main:

    ./sound-decipher ../GivenCounts/corpus.spanish.sound_bigrams.txt
    ../GivenCounts/corpus.spanish.quixote.written.txt

CypherReader can be compiled and run independently:

    clang++ -std=c++11 -stdlib=libc++ CypherReader.cc

### Examples
TODO

## Caveats
Make sure tags do not have apostrophe in them.
