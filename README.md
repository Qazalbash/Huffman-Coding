# Huffman Coding

Huffman Coding is a lossless data compression algorithm. The idea is to assign variable length codes to input characters, lengths of the assigned codes are based on the frequencies of corresponding characters. The most frequent character gets the smallest code and the least frequent character gets the largest code. The variable length codes assigned to input characters are Prefix Codes, means the codes (bit sequences) are assigned in such a way that the code assigned to one character is not the prefix of code assigned to any other character. This is how Huffman Coding makes sure that there is no ambiguity when decoding the generated bitstream. 

## Example

Consider the following message to be encoded: 
``` 
A spectre is haunting Europe – the spectre of communism. All the powers of old Europe have entered into a holy alliance to exorcise this spectre: Pope and Tsar, Metternich and Guizot, French Radicals and German police-spies.

Where is the party in opposition that has not been decried as communistic by its opponents in power? Where is the opposition that has not hurled back the branding reproach of communism, against the more advanced opposition parties, as well as against its reactionary adversaries?

Two things result from this fact:

I. Communism is already acknowledged by all European powers to be itself a power.

II. It is high time that Communists should openly, in the face of the whole world, publish their views, their aims, their tendencies, and meet this nursery tale of the Spectre of Communism with a manifesto of the party itself.

To this end, Communists of various nationalities have assembled in London and sketched the following manifesto, to be published in the English, French, German, Italian, Flemish and Danish languages.
```
The frequencies of the characters are as follows: 

| Char | Count | Char | Count | Char | Count | Char | Count |
|------|-------|------|-------|------|-------|------|-------|
|      | 165   | G    | 3     | c    | 25    | o    | 64    |
| ,    | 15    | I    | 5     | d    | 27    | p    | 29    |
| -    | 1     | L    | 1     | e    | 96    | r    | 44    |
| .    | 7     | M    | 1     | f    | 16    | s    | 66    |
| :    | 2     | P    | 1     | g    | 11    | t    | 72    |
| ?    | 2     | R    | 1     | h    | 47    | u    | 20    |
| A    | 2     | S    | 1     | i    | 71    | v    | 6     |
| C    | 4     | T    | 3     | k    | 3     | w    | 12    |
| D    | 1     | W    | 2     | l    | 33    | x    | 1     |
| E    | 4     | a    | 63    | m    | 29    | y    | 9     |
| F    | 3     | b    | 10    | n    | 61    | z    | 1     |

Huffman Coding assigns the following codes to the characters:

| Char | Code       | Char | Code       | Char | Code      | Char | Code       |
|------|------------|------|------------|------|-----------|------|------------|
|      | 111        | G    | 110110001  | c    | 00110     | o    | 1000       |
| ,    | 011101     | I    | 10110111   | d    | 00111     | p    | 01000      |
| -    | 1011010000 | L    | 1011010001 | e    | 000       | r    | 11010      |
| .    | 0111000    | M    | 1011010110 | f    | 101100    | s    | 1001       |
| :    | 011100111  | P    | 1011010011 | g    | 1101101   | t    | 1100       |
| ?    | 011100110  | R    | 1101100001 | h    | 0010      | u    | 101111     |
| A    | 101101010  | S    | 1101100000 | i    | 1010      | v    | 11011001   |
| C    | 10110110   | T    | 110111011  | k    | 110111010 | w    | 1101111    |
| D    | 1011010010 | W    | 1101110011 | l    | 01111     | x    | 1101110010 |
| E    | 01110010   | a    | 0110       | m    | 01001     | y    | 1011100    |
| F    | 110111000  | b    | 1011101    | n    | 0101      | z    | 1011010111 |

