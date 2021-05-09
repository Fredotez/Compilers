Assignment 3, CST152.
Frederic Desjardins
04094359

/************* Testing Results ************************/

All standard inputs been tested. Outputs seem to be matching the ones provided.
I have found 2 bugs,

The first seems to affect the line number. In certain cases, the line number increments a bit too much or not enough.

The 2nd is with the SEOF symbol, although it detects it correctly, it prints 255 instead of 0.

Fixed a bug where the buffer capacity was always 1 more then it should be. 

