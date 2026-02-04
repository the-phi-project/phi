[![Visual Studio Code](https://custom-icon-badges.demolab.com/badge/Visual%20Studio%20Code-0078d7.svg?logo=vsc&logoColor=white)](#)
[![Read the Docs](https://img.shields.io/badge/Read%20the%20Docs-8CA1AF?logo=readthedocs&logoColor=fff)](#)
[![macOS](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=F0F0F0)](#)
[![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)](#)

[![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=fff)](#)
[![C++](https://img.shields.io/badge/-C++-blue?logo=cplusplus)](#)
[![SQLite](https://img.shields.io/badge/SQLite-%2307405e.svg?logo=sqlite&logoColor=white)](#)

[![version](https://img.shields.io/badge/version-0.2.2-blue)](#)
[![passing](https://img.shields.io/badge/build-passing-green)](#)
[![testing](https://img.shields.io/badge/coverage-N/A-yellow)](#)
[![secure](https://img.shields.io/badge/secure-very-green)](#)


<div align="center">

<img src="https://raw.githubusercontent.com/TrynaThinkOf1/phi/e8bea8aebf333dbea9d54ebae7892afb50c9369d/assets/phi_logo.svg" alt="logo" width="250">

# PHI

<h5 style="color:#fbb040">Quantum-Safe TUI Encryption/Decryption Tool<br>Named after Pheidippides <i>(Φειδιππίδης)</i> who ran 25 miles from Marathon to Athens in order to deliver a message.</h5>

</div>
> [!NOTE]
> Guys I promise that I will profile this and make it as memory efficient as possible
> But I have to build the thing first 😖

<h1>📊 FEATURES</h1>

<h5>👤 USER EXPERIENCE</h5>

* SQLite3 database to hold user data - [SQLiteC++](https://github.com/SRombauts/SQLiteCpp)
	* Password protected
* Advanced and intuitive Terminal User Interface - [FTXUI](https://arthursonzogni.github.io/FTXUI/)
	* Built in screensaver (Credit: Oakamoore) - [TERMINAL-RAIN](https://github.com/Oakamoore/terminal-rain/)

<h5>🔒 SECURITY</h5>

> [!IMPORTANT]
> Database API is primed for password protection, the only roadblock is SQLiteC++ and CMake

* GZip (L3) compression for messages - [zlib](https://zlib.net)
* Supports multiple symmetric encryption algorithms for message content
	* ChaCha20-Poly1305 - [libsodium](https://doc.libsodium.org/secret-key_cryptography/aead/chacha20-poly1305)
	* AES-128, AES-192, AES-256 - [Crypto++](https://www.cryptopp.com/wiki/Advanced_Encryption_Standard)
* Supports multiple asymmetric encryption algorithms for message blocks
	* RSA-2048, RSA-4096 - [Crypto++](https://www.cryptopp.com/wiki/RSA_Cryptography)
	* Kyber-512, Kyber-768 - [pq-crystals](https://github.com/pq-crystals/kyber)
* Provides performant and secure hybrid encryption, inspiration from PGP
* BLAKE2b signature to ensure no tampering - [Crypto++](https://www.cryptopp.com/wiki/BLAKE2)

<h1>⤵️ INSTALLATION</h1>

Download the latest prebuilt version matching your OS and architecture in the releases tab.
Unfortunately, Phi is not available for windows yet. Coming soon.

<h3>BUILD YOURSELF</h3>

<h5>LINUX (Debian/Ubuntu)</h5>

```bash
sudo apt install zlib1g-dev  # install zlib

git clone https://www.github.com/TrynaThinkOf1/phi.git
cd phi/

python3 sysutils.py  # select option 2

sudo mv phi /usr/bin/phi

phi  # then proceed with setup
```

<h5>MACOS</h5>

```bash
git clone https://www.github.com/TrynaThinkOf1/phi.git
cd phi/

python3 sysutils.py  # select option 2

sudo mv phi /usr/local/bin/phi

echo 'PATH="/usr/local/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```