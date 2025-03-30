# momoisay
*A CLI program written in C featuring talking Saiba Momoi from Blue Archive!!!*

[![License](https://img.shields.io/badge/license-GPL--3.0-blue)]()
[![Version](https://img.shields.io/badge/version-1.0.1--beta.1-green)]()

---

## Features
- Talking ASCII art of Momoi
- Animated ASCII art of Momoi

## Installation

### Linux
```bash
git clone https://github.com/Mon4sm/momoisay.git
cd Momoisay
sudo sh ./install/linux.sh
```
### MacOS
```bash
git clone https://github.com/Mon4sm/momoisay.git
cd Momoisay
sudo sh ./install/mac.sh
```
## Usage
```bash
momoisay --help          # Show help
momoisay <text>         # Simple Usage
momoisay -a -n            # Animated Momoi with no text bubble (default version 1)
```

## File Structure
```
repo/
├── bin/ 
│    └── linux/
│    │   └── momoisay
│    └── mac/
│        └── momoisay
├── install/
│    ├── mac.sh
│    └── linux.sh
├── src/
│    ├── art/
│    │   ├── art.c
│    │   └── art.h
│    └── momoisay.c
├── LICENSE
└── README.md
```
