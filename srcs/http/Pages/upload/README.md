*This project has been created as part of the 42 curriculum by aaichache, mlouron, shessoun.*
## 🕸️ Description
`webserv` is a 42 school project whose goal is to recreate an **HTTP server** in C++, inspired by the behavior of **Nginx**. The program must be able to handle HTTP requests, serve static files, and execute CGI scripts, while complying with the HTTP/1.1 protocol standards.
This project focuses on:
* system programming
* socket management
* connection multiplexing
* code rigor and robustness
---
## 🎯 Project Goals
* Understand how a web server works
* Manipulate TCP/IP sockets
* Implement the HTTP/1.1 protocol
* Handle multiple clients simultaneously without blocking
* Parse a configuration file
* Deepen object-oriented programming skills in C++
---
## ⚙️ Features
### HTTP Requests
* Supported methods:
  * `GET`
  * `POST`
  * `DELETE`
### Server Management
* Non-blocking server
* Multiplexing via `poll()` (or equivalent)
* Multiple port support
* Multiple *server blocks* support
### Configuration
* Configuration file inspired by Nginx
* Supported directives:
  * `listen`
  * `server_name`
  * `root`
  * `index`
  * `error_page`
  * `client_max_body_size`
  * `location`
### CGI
* Execution of CGI scripts (e.g., PHP, Python)
* Environment variable handling
* Server ↔ CGI communication via pipes
### Other Features
* File uploads
* Custom error pages
* Correct handling of HTTP status codes
---
## 🗂️ Project Structure
```
webserv/
├── conf/
│   └── default.conf
├── src/
│   ├── main.cpp
│   ├── Server/
│   ├── Client/
│   ├── Request/
│   ├── Response/
│   └── CGI/
├── include/
│   └── webserv.hpp
├── www/
│   └── index.html
├── Makefile
└── README.md
```
---
## 🚀 Build & Run
### Build
```bash
make
```
### Starting the server
```bash
./webserv conf/default.conf
```
If no configuration file is provided, a default one is used.
---
## 🌐 Usage
Once the server is running, you can access the site via your browser:
```
http://localhost:8080
```
Example tests with `curl`:
```bash
curl http://localhost:8080/
curl -X POST http://localhost:8080/upload
curl -X DELETE http://localhost:8080/file.txt
```
---
## 🧪 Tests
* Manual tests via browser
* Tests with `curl`
* Behavior comparison with Nginx
* Validation with invalid requests
---
## ⚠️ Constraints
* C++98 only
* Only authorized system functions
* No blocking functions (`fork`, `execve` allowed for CGI)
* A single `poll()` (or equivalent) to handle all connections
---
## 📚 Useful Resources
* RFC 7230–7235 (HTTP/1.1)
* Nginx documentation
* `man poll`, `man socket\`, `man bind`
---
## ✅ Status
🚧 Work in progress
