ENTRY(main)
OUTPUT_FORMAT("binary")
physical = 0x00100000;

SECTIONS {
    . = physical;
    
    .entry      : { __entry_start = .;      *(.entry)   }
    .text       : { __text_start = .;       *(.text)    }
    .data       : { __data_start = .;       *(.data)    }
    .rodata     : { __rodata_start = .;     *(.rodata)  }
    .bss        : { __bss_start = .;        *(.bss)     }

    __end = .;
}