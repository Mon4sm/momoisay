
<div align="right">
  <details>
    <summary >🌐 Language</summary>
    <div>
      <div align="center">
        <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=en">English</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=zh-CN">简体中文</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=zh-TW">繁體中文</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=ja">日本語</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=ko">한국어</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=hi">हिन्दी</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=th">ไทย</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=fr">Français</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=de">Deutsch</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=es">Español</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=it">Italiano</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=ru">Русский</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=pt">Português</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=nl">Nederlands</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=pl">Polski</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=ar">العربية</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=fa">فارسی</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=tr">Türkçe</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=vi">Tiếng Việt</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=id">Bahasa Indonesia</a>
        | <a href="https://openaitx.github.io/view.html?user=Mon4sm&project=momoisay&lang=as">অসমীয়া</
      </div>
    </div>
  </details>
</div>

# momoisay
*A CLI program written in C featuring talking Saiba Momoi from Blue Archive!!!*

[![License](https://img.shields.io/badge/license-GPL--3.0-blue)](LICENSE)
[![Version](https://img.shields.io/badge/version-1.2.0-green)]()

---

## Preview

Example of animated usage

![Demo](preview.gif)

## Features
- Talking ASCII art of Momoi
- Animated ASCII art of Momoi
- Freestyle changing animation of Momoi

## Installation

#### disclaimer
Sometimes the precompiled binaries are not the latest version as the release. If you want to make sure that you got the latest version, you can built it from source. You can check the versions of the compiled binaries in the `/bin` folders.

### Linux
```bash
git clone https://github.com/Mon4sm/Momoisay.git
cd Momoisay
sudo sh ./install/linux.sh
```
### MacOS
```bash
git clone https://github.com/Mon4sm/Momoisay.git
cd Momoisay
sudo sh ./install/mac.sh
```
### Build from source (Latest Version)
```bash
git clone https://github.com/Mon4sm/Momoisay.git
cd Momoisay
make
```

## Usage
```bash
momoisay --help         # Show help
momoisay <text>         # Simple Usage
momoisay -a             # Animated Momoi with no text bubble (default version 1)
momoisay -a <version>   # Animated Momoi with no text bubble (default version 1)
momoisay -f             # Freestyle animations (This is pretty cool for ricing)
momoisay -c cyan -f     # Freestyle animations (But with custom colors!!!)
```

## File Structure
```
repo/
├── bin/ 
│    ├── linux/
│    │   ├── version.txt
│    │   └── momoisay
│    └── mac/
│        ├── version.txt 
│        └── momoisay
├── install/
│    ├── mac.sh
│    └── linux.sh
├── src/
│    ├── art/
│    │   ├── art.c
│    │   └── art.h
│    ├── extern/
│    │   ├── cString.c
│    │   └── cString.h
│    ├── momoisay.c
│    ├── render.c
│    ├── render.h
│    ├── speech.c
│    └── speech.h
├── Makefile
├── LICENSE
├── preview.gif
└── README.md
```
