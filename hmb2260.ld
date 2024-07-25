ENTRY(_Reset)
SECTIONS
{
. = 0x81000000;
.startup . : { startup.o(.text) }
.handler : ALIGN(4)
 { 
 	. = ALIGN (0x100);
 	KEEP(*(.handler))
 	. = ALIGN (8);
  }
.text : ALIGN(4)
 { 
 	*(.text) 
 }
 .data : ALIGN (4)
 { 
    . = ALIGN (4);
    *(.data)
    . = ALIGN (4);
 }
.bss : { *(.bss) }
. = . + 0x8000; /* 16kB of stack memory */
stack_top = .;
}