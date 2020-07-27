# three-bite-comp
Uses file size and (re)occurance to store every second occurance of a 4-byte word in 3-bytes
<BR><BR>
<B>Requirements</B>
<UL><LI>gcc</LI></UL>
<BR>
<B>Compiling:</B><BR>
<UL><LI>gcc three-byte-comp.c -o comp</LI>
<LI>gcc three-byte-comp.c -DDECOMPRESSOR -o decomp</LI></UL>
<BR>
<B>Optional Flags:</B>
<UL><LI>-DINFO for extra debugging information (slow)</LI>
<LI>-Ofast for compiler optimizations</LI>
<LI>-g for gdb debugging support</LI></UL>
<BR>
<B>Usage:</B><BR>
  <UL><LI>./comp <I>filename</I> (<I>number of iterations</I>)</LI>
<LI>./decomp <I>filename</I> (<I>number of iterations</I>)</LI></UL>
