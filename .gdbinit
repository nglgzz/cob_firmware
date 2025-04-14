# https://cgi.cse.unsw.edu.au/~learn/debugging/modules/gdb_init_file/

file final.elf
target extended-remote localhost:3333

monitor reset init
monitor flash write_image final.elf
monitor reset halt

info functions

define fns
    info functions
end
