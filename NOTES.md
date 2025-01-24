```
head big_artists.json -n 10000 > artists.json 

valgrind --tool=callgrind ./ArchiveFiles 
/usr/bin/kcachegrind
```