# CubeSat OBC


**STM32H753 on-board computer firmware and hardware for a CubeSat.**

Housekeeping, telecommand, hardware bring-up, and board design files for a CubeSat flight computer.

[![MCU](https://img.shields.io/badge/MCU-STM32H753ZI-blue)](https://www.st.com)
[![Firmware](https://img.shields.io/badge/firmware-1.1.0-green)](CubeSat_OBC_Firmware)
[![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-lightgrey)](https://www.st.com/en/development-tools/stm32cubeide.html)

## Short description

CubeSat OBC is an STM32H753ZI on-board computer project with CubeSat-style flight software (command, housekeeping, SAFE/NOMINAL/TEST modes), board tests, and hardware design files.

## Topics / hashtags

Use these as GitHub **Topics** (About → Topics) or post tags:

`#CubeSat` `#OBC` `#OnBoardComputer` `#STM32` `#STM32H7` `#STM32H753` `#FlightSoftware` `#Housekeeping` `#Telecommand` `#Telemetry` `#SpaceSystems` `#EmbeddedSystems` `#CANBus` `#FDCAN` `#Satellite` `#RemoteSensing`

GitHub topic names (no `#`):

```text
cubesat, obc, stm32, stm32h7, stm32h753, flight-software, housekeeping,
telecommand, telemetry, embedded-systems, can-bus, fdcan, satellite, remote-sensing
```

## Features

- STM32H753ZI Cortex-M7 flight computer
- Cooperative tasks: command, board heartbeat, housekeeping
- USART3 console (`ping`, `hk`, `mode`, hardware tests)
- Binary TM/TC frames: `A5 5A | op | len | payload | CRC-16`
- SAFE / NOMINAL / TEST operating modes
- Dual CAN, external SRAM, RTC, GPIO, UART, analogue inputs
- Schematic, PCB, BOM, and Gerbers

## Repository layout

| Path | Contents |
| --- | --- |
| [`CubeSat_OBC_Firmware/`](CubeSat_OBC_Firmware) | STM32CubeIDE project and flight / test firmware |
| [`CubeSat_OBC_Hardware/`](CubeSat_OBC_Hardware) | Schematic, PCB, BOM, Gerbers |
| [`CubeSat_OBC_Assets/`](CubeSat_OBC_Assets) | Console and setup images |

## Board

- STM32H753ZIT, 8 MB external SRAM (FMC)
- ST-LINK SWD + virtual COM port
- FDCAN1 / FDCAN2 (120 Ω termination)
- UART2, UART4, USART3 (115200 8-N-1)
- 8 thermistor / analogue inputs, 31 GPIO
- USB 3.3 V regulator, RTC, 2 user buttons, 3 LEDs

## Quick start

1. Clone this repository.
2. Open `CubeSat_OBC_Firmware` in **STM32CubeIDE**.
3. Build **Debug** and flash with ST-LINK.
4. Serial terminal: **115200 8-N-1** on the ST-LINK VCP.
5. Type `help` or `hk` and press Enter.

Firmware details: [`CubeSat_OBC_Firmware/README.md`](CubeSat_OBC_Firmware/README.md).

```text
  ============================================================
   CubeSat OBC
   On-Board Computer  |  STM32H753ZI  |  FW 1.1.0
  ============================================================
```

## Console commands

| Command | Action |
| --- | --- |
| `ping` | `PONG` |
| `ver` | Firmware version |
| `hk` | Housekeeping (uptime, boot count, RTC, mode) |
| `mode safe\|nominal\|test` | Flight mode (tests blocked in SAFE) |
| `led on\|off` | Status LED |
| `A` … `T` | Hardware tests |

Binary protocol: `A5 5A | op | len | payload | crc16_le` (CRC-16/CCITT-FALSE).

## Hardware files

- [Schematic](CubeSat_OBC_Hardware/CAD/CubeSat_OBC_Schematic.sch)
- [PCB CAD](CubeSat_OBC_Hardware/CAD/CubeSat_OBC_PCB.brd)
- [Gerbers](CubeSat_OBC_Hardware/Gerbers/CubeSat_OBC_Gerber.zip)

MCU docs: ST DS12117, RM0433.

## Firmware map

| File | Role |
| --- | --- |
| `obc_sched.c` | Cooperative tasks (CMD, BOARD, HK) |
| `obc_cmd.c` | Line commands and binary frames |
| `obc_hk.c` | Housekeeping and modes |
| `obc_crc16.c` | CRC-16/CCITT-FALSE |
| `board.c` / `console.c` | LEDs, buttons, USART3 |
| `ext_memory.c` / `hw_tests.c` / `can_app.c` | Board tests |

## License

- Application firmware: use and modify for the CubeSat OBC project.
- `Drivers/` (STM32 HAL and CMSIS): STMicroelectronics licenses in those folders.

## Author

CubeSat OBC
