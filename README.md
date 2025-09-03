![Preview1](demo/1.png)

# WoiWidgets Qt C++

![Qt](https://img.shields.io/badge/Qt-6.9.0-green?style=flat)
![MinGW](https://img.shields.io/badge/MinGW-15.1.0-blue?style=flat)
![OS](https://img.shields.io/badge/OS-Windows-lightgrey?style=flat)

---


## About project

This repository provides a Windows window template with a **custom title bar** that preserves **native functions** such as resize, minimize, maximize, and close.

In addition, it implements a basic system of a **custom proxy style**, including support for blur in menus and popup widgets. This solution serves as a convenient foundation for developers who want to build a modern and visually appealing interface based on Qt.

### Preview
<p align="center">
  <img src="demo/2.png" alt="Preview2" width="450"/>
  <img src="demo/3.png" alt="Preview3" width="450"/>
</p>

---

## Quick Start

```cpp
#include <QtWidgets>

#include "CustomWindow.h"
#include "core/style/WoiStyle.h"
#include "core/WoiThemeEngine.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle(new WoiStyle("Fusion"));

    WoiThemeEngine::applyDarkTheme();

    CustomWindow window(new QMainWindow());
    window.resize(800, 480);
    window.setWindowTitle("Window");

    window.show();
    return QApplication::exec();
}
```

> A more detailed example, where the **function of adding widgets to the window title bar** is implemented, can be found in [main.cpp](main.cpp).

---

## Demo

[![Watch the demo](demo/1.png)](demo/demo.mp4)
