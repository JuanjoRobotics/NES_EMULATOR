# NES Emulator

A Nintendo Entertainment System (NES) emulator written in C++.

## Project Structure

```plaintext
├── src/                    # Source code
│   ├── core/              # Core emulator components
│   │   ├── cpu.cpp        # 6502 CPU implementation
│   │   ├── ppu.cpp        # Picture Processing Unit
│   │   ├── apu.cpp        # Audio Processing Unit
│   │   ├── memory.cpp     # Memory management
│   │   ├── cartridge.cpp  # Cartridge/ROM handling
│   │   └── nes.cpp        # Main NES system class
│   ├── mappers/           # Memory mappers
│   │   ├── mapper000.cpp  # NROM mapper
│   │   ├── mapper001.cpp  # SxROM mapper
│   │   └── ...            # Other mappers
│   ├── gui/               # User interface
│   ├── audio/             # Audio output
│   ├── input/             # Input handling
│   ├── utils/             # Utility functions
│   └── main.cpp           # Entry point
├── include/               # Header files (mirrors src structure)
├── tests/                 # Unit tests
├── roms/                  # ROM files (not included)
├── build/                 # Build artifacts
├── docs/                  # Documentation
└── assets/                # Resources (sprites, icons, etc.)
```

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Dependencies

- CMake 3.15+
- C++17 compatible compiler
- SDL2 (for graphics and input)
- OpenGL (optional, for hardware acceleration)

## Features

- [ ] 6502 CPU emulation
- [ ] PPU (Picture Processing Unit)
- [ ] APU (Audio Processing Unit)
- [ ] Memory mappers
- [ ] Controller input
- [ ] Save states
- [ ] ROM loading

## License

MIT License
