#!/bin/python3
"""
Module providing a method for booting custom code on a HMB2260V2 using
Motorola S-Records, since all other methods are disabled or removed in CFE.

Fore more details, visit https://github.com/mathijsvandenberg/hmb2260/
"""

import threading
import sys
import time
import os
import serial

BASE_ADDRESS = 0x81000000


def checksum(data):
    return ~sum(data) & 0xFF


def send_srec(file_name, ser):
    max_data_len = 32  # 32 bytes of data per S3 record
    file_stats = os.stat(file_name)
    cnt = 0

    with open(file_name, 'rb') as f:
        address = BASE_ADDRESS
        while True:
            data = f.read(max_data_len)
            cnt = cnt + max_data_len
            pct = (100*cnt)/file_stats.st_size
            if not data:
                break

            byte_count = len(data) + 5  # 1 byte for count + 4 bytes for address + data length
            address_bytes = address.to_bytes(4, 'big')
            record = [byte_count] + list(address_bytes) + list(data)
            record.append(checksum(record))

            srec_record = f"S3{byte_count:02X}{address:08X}{''.join(f'{byte:02X}' for byte in data)}{record[-1]:02X}\n"
            ser.write(srec_record.encode('ascii'))
            sys.stdout.write(f"\r{hex(BASE_ADDRESS+cnt)} [{round(pct, 2)}%]")
            sys.stdout.flush()
            address += max_data_len

    # Add S7 termination record
    termination_record = f"S705{BASE_ADDRESS:08X}{checksum([5] + list(BASE_ADDRESS.to_bytes(4, 'big'))):02X}\n"
    ser.write(termination_record.encode('ascii'))
    ser.write('\n'.encode('ascii'))
    return


# Function to be called when 'CFE> ' is found
def handle_prompt_found(file_name, ser):
    print("Sending binary file by using srec...")
    data = "boot -srec -nz uart0:hmb2260loader\n"
    ser.write(data.encode('ascii'))
    time.sleep(0.5)
    send_srec(file_name, ser)
    print("\nBye!")
    sys.exit()


def read_from_port(ser, file_name):
    buffer = ""
    myfile = open("dump.txt", "a", encoding="ascii")
    print("Ready! Now powerup the HMB2260 port and apply the 'NAND glitch' shortly!")

    while True:
        try:
            byte = ser.read(1).decode('ascii', errors='ignore')
            sys.stdout.write(byte)
            sys.stdout.flush()
            myfile.write(byte)
            buffer += byte
            if buffer.endswith('CFE> '):
                buffer = ""
                print("Trigger!")
                # Create and start reading thread
                trigger_thread = threading.Thread(target=handle_prompt_found, args=(file_name, ser,))
                trigger_thread.daemon = True
                trigger_thread.start()
        except KeyboardInterrupt:
            print("CTRL+C, bye!")
            sys.exit()


# Main function to set up and start threads
def main():
    if len(sys.argv) != 3:
        print("Usage: python3 hmb2260loader.py <serial_port> <binary_file>")
        print("e.g: python3 hmb2260loader.py COM10 hmb2260.bin")
        sys.exit()

    serial_port = sys.argv[1]
    file_name = sys.argv[2]

    # Open the serial port
    try:
        ser = serial.Serial(serial_port, 115200, timeout=1)
    except serial.serialutil.SerialException:
        print(f"Cannot open port {serial_port}")
        sys.exit()

    read_from_port(ser, file_name)
    sys.exit()


if __name__ == '__main__':
    main()
