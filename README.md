## What is this?
An implementation of the EM-based decipherment technique in [Knight and Yamada,
99](http://www.aclweb.org/anthology/W/W99/W99-0906.pdf) on Spanish sounds, given
separate IPA training material that is not parallel to historical text T (here,
Don Quixote).

### Purpose
Research. 

### Code Organization
- EMViterbiPackage contains the main code for running EM and Viterbi.
- GeneratedData contains example outputs.
- GivenCounts contains count data.

## Running
Compile using cmake. Example:

    mkdir build && cd build
    cmake ..
    make

Run the generated executable. Alternatively run any test executables, but make
sure you do `cmake -Dtest=ON ..` first.

Running Main:

    ./sound-decipher ../GivenCounts/corpus.spanish.sound_bigrams.txt
    ../GivenCounts/corpus.spanish.quixote.written_very_short.txt > output.txt

  Running on the very short sequence takes roughly 20 seconds. For long
  sequences, it can take quite a while.

CypherReader can be compiled and run independently:

    clang++ -std=c++11 -stdlib=libc++ CypherReader.cc

