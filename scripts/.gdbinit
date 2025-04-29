# Apply split layout with registers and code
tui new-layout splitv src 1 regs 1 status 0 cmd 1
tui new-layout splith {-horizontal src 2 regs 1} 2 status 0 {-horizontal cmd 1 asm 1} 1
set tui compact-source on
layout splith

# https://sourceware.org/gdb/current/onlinedocs/gdb.html/TUI-Keys.html#TUI-Keys
# C-x o     change active window
#
# C-x s     single key mode
#       c   continue
#       s   step
#       n   next (step over)
#       f   finish (step out of current function) ?
#       q   exit single key mode
#
# C-x a     enter / leave TUI mode
#
# C-x 1     TUI with one window (either 'source' or 'assembly')
#
# C-x 2     TUI with at leas two windows

# https://www.openocd.org/doc/html/General-Commands.html
# https://cgi.cse.unsw.edu.au/~learn/debugging/modules/gdb_init_file/
define .reload
    file bin/final.elf
    target extended-remote localhost:3333
    delete -y
    break main
    break GPIOTE_IRQHandler
end

define .flash
    monitor reset init
    monitor flash write_image final.elf
end

define .funcs
    info functions
end

define .vars
    info variables
end

# set *((unsigned int)0x[address]) = [value]

.reload
monitor reset halt
