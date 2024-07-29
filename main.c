#define UART_THR (*((volatile unsigned long *)0xB0406700))
#define UART_RBR (*((volatile unsigned long *)0xB0406700))
#define UART_IER (*((volatile unsigned long *)0xB0406704))
#define UART_IIR (*((volatile unsigned long *)0xB0406708))
#define UART_FCR (*((volatile unsigned long *)0xB040670C))
#define UART_LCR (*((volatile unsigned long *)0xB0406710))
#define UART_LSR (*((volatile unsigned long *)0xB0406714))

#define GPIO1 (*((volatile unsigned long *)0xB0409034))
#define GPIO2 (*((volatile unsigned long *)0xB0409010))
#define GPIO3 (*((volatile unsigned long *)0xB0409014))
#define GPIO4 (*((volatile unsigned long *)0xB040900C))
#define GPIO5 (*((volatile unsigned long *)0xB0409008))

#define DISPLAY1 (*((volatile unsigned long *)0xB0409018))
#define DISPLAY0 (*((volatile unsigned long *)0xB040901C))
#define DISPLAY3 (*((volatile unsigned long *)0xB0409020))
#define DISPLAY2 (*((volatile unsigned long *)0xB0409024))
#define LEDS (*((volatile unsigned long *)0xB040902C))

#define INTFC_STATUS (*((volatile unsigned long *)0xB0412814))

void exception_handler(void) __attribute__((section(".handler")));

void putch(char data) {
  while ((UART_LSR & 0x20) != 0x20) {
  }
  UART_THR = data;
}

void print(char *data) {
  for (int x = 0; data[x] != 0x00; x++) {
    putch(data[x]);
  }
}

void printnibble(unsigned char c) {
  switch (c) {
  case 0:
    putch('0');
    break;
  case 1:
    putch('1');
    break;
  case 2:
    putch('2');
    break;
  case 3:
    putch('3');
    break;
  case 4:
    putch('4');
    break;
  case 5:
    putch('5');
    break;
  case 6:
    putch('6');
    break;
  case 7:
    putch('7');
    break;
  case 8:
    putch('8');
    break;
  case 9:
    putch('9');
    break;
  case 10:
    putch('A');
    break;
  case 11:
    putch('B');
    break;
  case 12:
    putch('C');
    break;
  case 13:
    putch('D');
    break;
  case 14:
    putch('E');
    break;
  case 15:
    putch('F');
    break;
  default:
    putch('X');
    break;
  }
}

void hex(unsigned char c) {
  printnibble((c >> 4) & 0x0F);
  printnibble((c >> 0) & 0x0F);
}

void hex32(unsigned long a) {
  putch('0');
  putch('x');
  hex(a >> 24);
  hex(a >> 16);
  hex(a >> 8);
  hex(a >> 0);
}

void setmem(unsigned long address, unsigned long value) {
  *((volatile unsigned long *)(address | 0xa0000000)) = value;
}

void setmem2(unsigned long address, unsigned long value) {
  *((volatile unsigned long *)(address)) = value;
}

unsigned char mem(unsigned long address) {
  unsigned char result = 0;
  unsigned long remainder = address % 4;
  address = address - remainder;
  unsigned long data = *((volatile unsigned long *)address);

  switch (remainder) {
  case 0:
    result = (data >> 0) & 0xFF;
    break;
  case 1:
    result = (data >> 8) & 0xFF;
    break;
  case 2:
    result = (data >> 16) & 0xFF;
    break;
  case 3:
    result = (data >> 24) & 0xFF;
    break;
  }
  return result;
}

void uint32(unsigned long a) {
  unsigned long v = mem(a);
  for (int x = 1000000000; x > 0; x = x / 10) {
    unsigned long b = v / x;
    printnibble(b);
    v = v - (b * x);
  }
}

void dump(unsigned long address, unsigned long len) {
  unsigned long x = 0;
  unsigned long a = 0;
  for (a = address; a < address + len; a = a + 0x10) {
    hex32(a);
    putch(':');
    putch(' ');

    for (x = 0; x < 16; x++) {

      hex(mem(a + x));
      putch(' ');
    }
    putch(' ');
    putch(' ');
    putch(' ');
    for (x = 0; x < 16; x++) {
      unsigned char data = mem(a + x);

      if (data > 31 && data < 127) {
        putch(data);
      } else {
        putch('.');
      }
    }
    putch('\n');
  }
}

void dumpnand(unsigned long address) {
  unsigned long x = 0;
  unsigned long a = 0;
  unsigned long aa = 0;
  for (a = 0xB0412C00; a < 0xB0412E00; a = a + 0x10) {
    hex32(address + aa);
    putch(':');
    putch(' ');

    for (x = 0; x < 16; x++) {

      hex(mem(a + x));
      putch(' ');
    }
    putch(' ');
    putch(' ');
    putch(' ');
    for (x = 0; x < 16; x++) {
      unsigned char data = mem(a + x);

      if (data > 31 && data < 127) {
        putch(data);
      } else {
        putch('.');
      }
    }
    putch('\n');
    aa = aa + 16;
  }
}

void reg(unsigned long address) {
  unsigned long x = 0;
  unsigned long a = address;
  hex32(a);
  putch(':');
  putch(' ');

  for (x = 0; x < 4; x++) {
    hex(mem(a + x));
    putch(' ');
  }
  putch('\n');
}

void compare(unsigned long address, unsigned long value) {
  unsigned long data = *((volatile unsigned long *)address);
  if (data == value) {
    dump(address, 0x10);
  }
}

void delay(unsigned long a) {
  for (volatile int x = 0; x < a; x++) {
  }
}

unsigned long int2seg(int i) {
  unsigned long s = 0;
  switch (i) {
  case 0:
    s = 0b00111111;
    break;
  case 1:
    s = 0b00000110;
    break;
  case 2:
    s = 0b01011011;
    break;
  case 3:
    s = 0b01001111;
    break;
  case 4:
    s = 0b01100110;
    break;
  case 5:
    s = 0b01101101;
    break;
  case 6:
    s = 0b01111101;
    break;
  case 7:
    s = 0b00000111;
    break;
  case 8:
    s = 0b01111111;
    break;
  case 9:
    s = 0b01101111;
    break;
  case 'A':
    s = 0b01110111;
    break;
  case 'E':
    s = 0b01111001;
    break;
  case 'L':
    s = 0b00111000;
    break;
  case 'F':
    s = 0b01110001;
    break;
  case 'b':
    s = 0b01111100;
    break;
  case 't':
    s = 0b01111000;
    break;
  case ' ':
    s = 0b00000000;
    break;
  }
  return ~s;
}

void printreg(char *text, unsigned long addr) {
  print(text);
  putch(':');
  putch(' ');
  reg(addr | 0xA0000000);
}

void exception_handler() {
  DISPLAY0 = int2seg('F');
  DISPLAY1 = int2seg('A');
  DISPLAY2 = int2seg(1);
  DISPLAY3 = int2seg('L');
}

void readnand(unsigned long address) {
  int i = 0;
  setmem(0x1041280C, address);    // CMD_READ_ADDR
  setmem(0x10412804, 0x01000000); // CMD_START
  while ((INTFC_STATUS & 0xF0000000) != 0xF0000000) {
    if (++i == 1280) // Arbitrarily chosen
    {
      unsigned long if_status = INTFC_STATUS;

      print("NAND FLash will not get ready: ");
      print("INTFC_STATUS=");
      hex32(if_status);
      print("\n");
    }
  }
}

void set_uart_baudrate(unsigned long baudrate) {
  unsigned long dlat_high = 0;
  unsigned long dlat_low = 5068800 / baudrate;

  setmem(0x1040670C, 0x80);
  setmem(0x10406704, dlat_high);
  setmem(0x10406700, dlat_low);
  setmem(0x1040670C, 0x03);
}

void seg7_fade() {
  for (unsigned long i = 0; i < 256; i++) {
    GPIO2 = i;
    delay(100000);
  }
  for (unsigned long i = 0; i < 256; i++) {
    GPIO2 = 255 - i;
    delay(100000);
  }
}
void seg7_splash_screen() {
  for (int x = 0; x < 2; x++) {
    DISPLAY0 = int2seg('E');
    DISPLAY1 = int2seg('L');
    DISPLAY2 = int2seg('F');
    DISPLAY3 = int2seg(' ');
    seg7_fade();
    DISPLAY0 = int2seg('b');
    DISPLAY1 = int2seg(0);
    DISPLAY2 = int2seg(0);
    DISPLAY3 = int2seg('t');
    seg7_fade();
  }
}
void c_entry() {
  print("HMB2260 Loader v0.01 MvdB 2024\n");
  printreg("CHIP_FAMILY_ID ", 0x10404000);
  printreg("PRODUCT_ID     ", 0x10404004);
  printreg("TIMER0_CTRL    ", 0x104066C8);
  printreg("TIMER1_CTRL    ", 0x104066CC);
  setmem(0x104066CC, 0xCF000000);
  printreg("TIMER1_CTRL    ", 0x104066CC);
  printreg("TIMER2_CTRL    ", 0x104066D0);

  printreg("TIMER0_STAT    ", 0x104066D8);

  printreg("TIMER0_STAT    ", 0x104066D8);
  printreg("TIMER0_STAT    ", 0x104066D8);
  printreg("TIMER1_STAT    ", 0x104066DC);
  printreg("TIMER2_STAT    ", 0x104066E0);
  printreg("UART0_LCR      ", 0x1040670C);
  printreg("UART0_THR      ", 0x10406700);
  printreg("UART0_IER      ", 0x10406704);

  // Boot success! Some 7seg notification to show we reached this point
  seg7_splash_screen();

  // set UART baudrate
  set_uart_baudrate(115200);

  // Set exception vector to 0x81000100 (see linker script)
  for (unsigned long x = 0; x < 0x5C00; x = x + 0x10) {
    setmem2(0x80000000 + x, 0x3c1b8100);
    setmem2(0x80000004 + x, 0x277b0100);
    setmem2(0x80000008 + x, 0x03600008);
    setmem2(0x8000000C + x, 0x241a0008);
  }

  printreg("FLASH_DEV_ID   ", 0x10412994);
  printreg("FLASH_DEV_ID_EX", 0x10412998);
  print("Start Dump\n");

  for (unsigned long a = 0x0; a < 0x20000000;
       a = a + 0x200) // 1.000.000 x 512 bytes = 512MB
  {
    readnand(a);
    dumpnand(a);
  }
}
