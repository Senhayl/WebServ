*This project has been created as part of the 42 curriculum by aaichache, mlouron, shessoun.*

## Description

`webserv` is a C++98 HTTP server project from 42.
The objective is to implement a browser-compatible web server handling core HTTP features:

- HTTP request parsing and response generation
- static file serving
- methods `GET`, `POST`, `DELETE`
- configuration file parsing (Nginx-like blocks)
- route-based behavior (`location`)
- CGI execution (Python/PHP examples)

Current implementation status is **work in progress**.

## Instructions

### Requirements

- Linux or Unix-like environment
- `c++` compiler supporting C++98
- `make`

### Build

```bash
make
```

### Run

```bash
./webserv ./srcs/config/default.conf
```

You can also run without argument to use the default path:

```bash
./webserv
```

### Useful local tests

```bash
curl -i http://localhost:8080/
curl -i -X POST http://localhost:8080/upload/test.txt -d 'hello'
curl -i -X DELETE http://localhost:8080/upload/test.txt
curl -i http://localhost:8080/cgi-bin/hello.py
```

You can also compare behaviors with NGINX for status codes and headers.

## Features (Current Scope)

- Single binary: `webserv`
- Configuration directives parsed:
  - `listen`, `server_name`, `root`, `index`, `error_page`, `client_max_body_size`
  - `location`, `allowed_methods`, `return`, `upload_path`, `autoindex`, `cgi_extension`, `cgi_path`
- Default error pages available in `srcs/http/ErrorPages/`
- CGI examples included:
  - Python: `srcs/http/Pages/cgi-bin/hello.py`
  - PHP: `srcs/http/Pages/cgi-bin-php/calc.php`

## Project Layout

```text
.
├── Makefile
├── srcs/
│   ├── main.c++
│   ├── config/
│   ├── server/
│   └── http/
└── README.md
```

## Resources

### Classic references

- RFC 7230, RFC 7231 (HTTP/1.1 syntax and semantics)
- NGINX documentation (for behavior comparison)
- Linux man pages: `poll(2)`, `socket(2)`, `accept(2)`, `send(2)`, `recv(2)`, `fork(2)`, `execve(2)`, `waitpid(2)`

### AI usage in this project

AI tools were used as **assistant support**, mainly for:

- reviewing parser edge cases and proposing test ideas
- generating checklists for HTTP status/error handling
- helping draft and refine documentation text

All generated suggestions were manually reviewed, discussed between teammates, and adapted before integration.
No AI-generated code was copied blindly without understanding and validation.

## Status

🚧 Mandatory part is still being finalized and hardened for full subject compliance.
