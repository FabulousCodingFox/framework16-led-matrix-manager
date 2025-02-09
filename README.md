<h1 align="center">Framework16 LED Matrix Manager</h1>
<p align="center"><i>A GUI tool for managing Framework16 LED displays</i></p>

<p align="center">
  <!-- Badges -->
  <a href="https://github.com/FabulousCodingFox/framework16-led-matrix-manager/stargazers">
    <img alt="GitHub Stars" src="https://img.shields.io/github/stars/FabulousCodingFox/framework16-led-matrix-manager?style=for-the-badge">
  </a>
  <a href="https://github.com/FabulousCodingFox/framework16-led-matrix-manager/network/members">
    <img alt="GitHub Forks" src="https://img.shields.io/github/forks/FabulousCodingFox/framework16-led-matrix-manager?style=for-the-badge">
  </a>
  <a href="https://github.com/FabulousCodingFox/framework16-led-matrix-manager/issues">
    <img alt="GitHub Issues" src="https://img.shields.io/github/issues/FabulousCodingFox/framework16-led-matrix-manager?style=for-the-badge">
  </a>
  <a href="https://github.com/FabulousCodingFox/framework16-led-matrix-manager/blob/main/LICENSE">
    <img alt="GitHub License" src="https://img.shields.io/github/license/FabulousCodingFox/framework16-led-matrix-manager?style=for-the-badge">
  </a>
</p>

---

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation 🚀](#installation-)
- [Running ▶️](#running-)
- [Development 🛠️](#development-)
- [Building 📦](#building-)
- [Screenshots 📸](#screenshots-)
- [Contributing 🤝](#contributing-)
- [License](#license)
- [Support ☕](#support-)

---

## Introduction

The **Framework16 LED Matrix Manager** is a GUI application designed to simplify the configuration and control of Framework16 LED displays. This tool is offering a straightforward interface and powerful features for displaying cool stuff on the displays.

---

## Features

- **Native GUI using QT:** Built with a modern QT interface for a responsive and robust user experience.
- **Easy Management:** Simplified controls for quick and efficient LED display management.
- **Advanced Presets:** Access and utilize a variety of presets.
- **First Class Linux Support:** Optimized for GNU/Linux environments, ensuring reliable performance on x86-64 systems.

---

## Installation 🚀

### Prerequisites

- [Nix Package Manager](https://nixos.org/download.html) installed on your system.
- Basic understanding of Git and terminal commands.

### Steps

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/FabulousCodingFox/framework16-led-matrix-manager.git
   cd framework16-led-matrix-manager
   ```

2. **Enter the Development Environment:**

   ```bash
   nix develop
   ```

   This command initializes a Nix shell with all the necessary dependencies without affecting your host system.

---

## Running ▶️

You have two methods to run the program:

- **Download Build Artifact:**  
  Download the pre-built artifact from the repository's releases. This artifact is ready to run on any generic GNU/Linux x86-64 system with QT installed.

- **Use Nix Run:**  
  Launch the application directly using Nix by executing:

  ```bash
  nix run
  ```

  This command will build (if necessary) and run the application within the Nix environment.

---

## Development 🛠️

To contribute or experiment with new features, start by entering the development environment:

```bash
nix develop
```

This isolated shell ensures that all dependencies are managed appropriately, allowing for a consistent development experience without modifying your main system environment.

---

## Building 📦

Below are the build instructions for different platforms:

### Linux

For GNU/Linux x86-64 systems, compile the package with:

```bash
nix build .#packages.x86_64-linux.default
```

### Windows

**Note:** The Windows build process is not supported at this time due to limitations with cross-compilation in `nixpkgs.qt6`. If cross-compilation becomes available in future updates, the following command may be applicable:

```bash
nix build .#packages.x86_64-windows.default
```

---

## Screenshots 📸

Below are some placeholder screenshots demonstrating the application’s interface:

<p align="center">
  <img src="https://via.placeholder.com/400x200?text=Screenshot+1" alt="Screenshot 1" style="margin: 10px;">
  <img src="https://via.placeholder.com/400x200?text=Screenshot+2" alt="Screenshot 2" style="margin: 10px;">
  <img src="https://via.placeholder.com/400x200?text=Screenshot+3" alt="Screenshot 3" style="margin: 10px;">
</p>

---

## Contributing 🤝

Contributions are welcome. To propose improvements or add new features, please follow these steps:

1. **Fork the Repository:** Click the "Fork" button at the top-right of this page.
2. **Create a Feature Branch:**  
   """
   git checkout -b feature/your-feature-name
   """
3. **Commit Your Changes:**  
   """
   git commit -m "feat: describe your changes here"
   """
4. **Push to Your Branch:**  
   """
   git push origin feature/your-feature-name
   """
5. **Open a Pull Request:** Provide a clear summary of your changes for review.

For additional guidelines, refer to our [CONTRIBUTING.md](CONTRIBUTING.md).

---

## License

This project is licensed under the Boost Software License. See the [LICENSE](LICENSE) file for details.

---

## Support ☕ 

If you find this project useful and would like to support its development, please consider contributing via Ko-fi:

[![Ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/fabulouscodingfox)

Your support is appreciated!

---

For further questions or to provide feedback, please open an issue or contact the maintainers directly.

Happy coding!
