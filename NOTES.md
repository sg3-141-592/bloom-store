```
head big_artists.json -n 10000 > artists.json 

clang-tidy --checks=* -p build Archive.cpp

valgrind --tool=callgrind ./ArchiveFiles 
/usr/bin/kcachegrind
```