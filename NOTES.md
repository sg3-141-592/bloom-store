```
Libraries
https://github.com/jvirkki/libbloom/blob/master/bloom.h

pstree 484049 --thread-names
pidstat -p 484676 1

head big_artists.json -n 10000 > artists.json 
rm -rf archive/*

clang-tidy --checks=* -p build Archive.cpp

valgrind --tool=callgrind ./ArchiveFiles 
/usr/bin/kcachegrind
```