```
Libraries
https://github.com/jvirkki/libbloom/blob/master/bloom.h

du -sh archive/
find archive/ -type f \( -name "*.bloom" -o -name "*.json.gz" \) -exec du -ch {} + | grep $total

pstree 498333 --thread-names
pidstat -p 498333 1
htop --pid 498333

sed -n 1103505p artist

time grep "Green Day" artist | jq '.type + " " + .name'een Day" artist | jq '.type + " " + .name'

head big_artists.json -n 10000 > artists.json 
rm -rf archive/*

LLVM_PROFILE_FILE="test_%p.profraw" ./test_archive_files
llvm-profdata merge -o coverage.profdata *.profraw
llvm-cov show ./test_archive_files \
    -instr-profile=coverage.profdata \
    -output-dir=coverage_report \
    -format=html \
    -ignore-filename-regex="_deps/*"
python -m http.server

clang-tidy -checks=bugprone-\*,modernize-\*,performance-\*,readability-\* --dump-config > .clang-tidy
clang-tidy --checks=* -p build Archive.cpp

valgrind --tool=callgrind ./ArchiveFiles 
/usr/bin/kcachegrind
```