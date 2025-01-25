```
Libraries
https://github.com/jvirkki/libbloom/blob/master/bloom.h

pstree 498333 --thread-names
pidstat -p 498333 1
htop --pid 498333

sed -n 1103505p artist

time grep "Green Day" artist | jq '.type + " " + .name'een Day" artist | jq '.type + " " + .name'

head big_artists.json -n 10000 > artists.json 
rm -rf archive/*

clang-tidy --checks=* -p build Archive.cpp

valgrind --tool=callgrind ./ArchiveFiles 
/usr/bin/kcachegrind
```