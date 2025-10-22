# C Pointer Lab — book-mapped scaffold
Each chapter folder contains:
- notes.md  | your takeaways
- experiments/ | tiny runnable demos
- bugs/ | intentional mistakes (caught by ASan/LLDB)
- kata/ | drills
- diagrams/ | ASCII / images
- logs/ | LLDB / ASan outputs

## Build & run
make run                # defaults to CH=01_intro
make asan CH=02_dynamic_memory
make lldb CH=03_functions
