# WebServ — Missing / To Do

Based on a careful read of `en.subject.pdf` and the current state of the codebase.

---

## 🔴 CRITICAL — Core architecture broken

### 1. Only one server / one port is started
- **File:** `srcs/main.c++` (comment says "1 serveur et 1 port pour linstant")
- **Subject:** "Your server must be able to listen to multiple ports" / "defining multiple websites served by your program"
- **Fix:** Loop over all `ServerConfig` objects, create one `Server` per `listen` entry, register all server fds in the same `poll()`.

### 2. Config is never passed to the request handlers
- **File:** `srcs/http/Response/MethodHandler.cpp`, `srcs/server/Loop.cpp`
- **Subject:** All routing (root, index, allowed_methods, upload_path, redirections, autoindex, CGI…) must come from the config
- **Fix:** Pass the matching `ServerConfig` + `Location` to `MethodHandler` so it can apply the correct rules.

### 3. poll() must monitor read AND write simultaneously
- **File:** `srcs/server/Loop.cpp`
- **Subject:** "poll() must monitor both reading and writing simultaneously"
- **Fix:** `client_poll.events = POLLIN | POLLOUT` and check `revents` flags separately instead of switching between the two.

---

## 🔴 CRITICAL — CGI not implemented at all

Config correctly parses `cgi_extension` and `cgi_path`, but there is **zero CGI execution** in the codebase.

### 4. CGI execution (fork / execve / pipes)
- **Subject:** CGI based on file extension (e.g. `.php`, `.py`); full request + args available to CGI; CGI runs in correct directory; output piped back to client
- **Missing pieces:**
  - `fork()` → child executes CGI binary via `execve()` with correct env vars
  - Pipes: parent writes request body to CGI stdin, reads CGI stdout back
  - CGI env vars: `REQUEST_METHOD`, `CONTENT_LENGTH`, `CONTENT_TYPE`, `QUERY_STRING`, `PATH_INFO`, `SCRIPT_FILENAME`, `SERVER_NAME`, `SERVER_PORT`, etc.
  - `chdir()` to script directory before exec
  - Handle CGI output: if no `Content-Length` returned, treat EOF as end of data
  - CGI fd must be added to `poll()` — never block on pipe read/write

### 5. Chunked Transfer-Encoding not decoded
- **File:** `srcs/server/Loop.cpp` (`handleClientRead`)
- **Subject:** "for chunked requests, your server needs to un-chunk them; the CGI will expect EOF as the end of the body"
- **Fix:** Detect `Transfer-Encoding: chunked` header; parse chunks (size\r\ndata\r\n...0\r\n\r\n) and reconstruct raw body before passing to handler.

---

## 🔴 CRITICAL — Routing and location rules not enforced

### 6. Allowed methods per location not checked
- **File:** `srcs/http/Response/MethodHandler.cpp` / `srcs/http/Request/RequestValidator.cpp`
- **Subject:** "List of accepted HTTP methods for the route"
- **Fix:** Look up the matching `Location` block for the request path; if the method is not in `allowed_methods`, return 405.

### 7. `root` directive not used — path is hardcoded
- **File:** `srcs/http/Response/MethodHandler.cpp` (hardcoded `"./srcs/http/Pages"`)
- **Subject:** "directory where the requested file should be located (e.g. /kapouet → /tmp/www)"
- **Fix:** Resolve file path from `Location::getRoot()` (or `ServerConfig::getRoot()`) + request URI.

### 8. `index` directive not used
- **File:** `srcs/http/Response/MethodHandler.cpp` (hardcoded `/index.html`)
- **Subject:** "default file to serve when the requested resource is a directory"
- **Fix:** When URI ends with `/` or resolves to a directory, serve `Location::getIndex()`.

### 9. HTTP redirections from config not applied
- **File:** `srcs/http/Response/MethodHandler.cpp` (only one hardcoded redirect for `/redirections.html`)
- **Subject:** "HTTP redirection" directive in location blocks
- **Fix:** After matching the location, check `Location::getReturn()` and if set, issue a 301/302 response with the `Location` header.

### 10. `upload_path` from config not used — upload path is hardcoded
- **File:** `srcs/http/Response/MethodHandler.cpp` (`"./srcs/http/Pages/upload"`)
- **Subject:** "uploading files from the clients to the server is authorized, and storage location is provided"
- **Fix:** Use `Location::getUploadPath()` to determine where to write the uploaded file.

### 11. `client_max_body_size` not enforced
- **File:** `srcs/server/Loop.cpp`
- **Subject:** "Set the maximum allowed size for client request bodies"
- **Fix:** After reading the body, compare its size against the value from config; if exceeded return 413.

### 12. Directory listing (autoindex) not implemented
- **File:** `srcs/http/Response/MethodHandler.cpp`
- **Subject:** "Enabling or disabling directory listing"
- **Fix:** When URI resolves to a directory and `autoindex on` in the location, generate an HTML page listing the directory contents (use `opendir` / `readdir`).

---

## 🟠 IMPORTANT — Error handling gaps

### 13. Default error pages from config not used
- **File:** `srcs/http/Response/HttpResponse.cpp` (`loadError` only looks at `./srcs/http/ErrorPages/{code}.html`)
- **Subject:** "Set up default error pages" per server block
- **Fix:** Pass server config to `createError`; check `ServerConfig::getErrorPages()` first, fall back to built-in pages.

### 14. Missing error pages (only 404.html provided)
- **Directory:** `srcs/http/ErrorPages/`
- **Subject:** "Your server must have default error pages if none are provided"
- **Fix:** Add at minimum: `400.html`, `403.html`, `405.html`, `413.html`, `500.html`, `501.html`, `505.html`.

### 15. `Content-Length` missing from responses
- **File:** `srcs/http/Response/HttpResponse.cpp` (`createResponse`)
- **Subject:** HTTP/1.1 requires `Content-Length` on all responses with a body
- **Fix:** Add `Content-Length: <body.size()>` header in `createResponse` and `createError`.

### 16. `errno` checked / used after read/write (verify)
- **Subject:** "Checking the value of errno to adjust the server behaviour is strictly forbidden after performing a read or write operation"
- **Fix:** Audit `Loop.cpp` — do not use `errno` after `read()` / `send()`.

---

## 🟠 IMPORTANT — Server infrastructure

### 17. No default config path fallback
- **File:** `srcs/main.c++`
- **Subject:** "provided as an argument on the command line, **or available in a default path**"
- **Fix:** If `ac == 1`, fall back to a default path (e.g. `./srcs/config/default.conf`).

### 18. Server does not handle partial `send()`
- **File:** `srcs/server/Loop.cpp` (`handleClientWrite`)
- **Current:** Sends the entire response in one `send()` call; ignores partial sends
- **Fix:** Track how many bytes have been sent; on next `POLLOUT` event continue sending from the offset.

### 19. Request can hang indefinitely
- **File:** `srcs/server/Loop.cpp`
- **Subject:** "A request to your server should never hang indefinitely"
- **Fix:** Add a timeout mechanism (e.g. timestamp per client; drop clients that haven't completed a request within N seconds).

### 20. A single server fd in poll — multiple listen ports require multiple server fds
- Related to point 1; when supporting multiple ports, `Loop::run()` must check all server fds in the `_fds` array, not just `_server.getFd()`.

---

## 🟡 MODERATE — Validator / parser issues

### 21. `isValidPath` opens the file on disk — couples validator to hardcoded path
- **File:** `srcs/http/Request/RequestValidator.cpp`
- Path validation should not depend on a hardcoded root; it should be done after location matching with the config root.

### 22. Chunked body not detected in `RequestParser`
- **File:** `srcs/http/Request/RequestParser.cpp`
- Body is concatenated with newlines — binary uploads will be corrupted; body must be read as raw bytes.

### 23. `HTTP/1.0` requests could be accepted by browsers or tools
- **File:** `srcs/http/Request/RequestValidator.cpp`
- Subject says HTTP/1.0 is a reference point; consider accepting it (respond with 1.1).

---

## 🟡 MODERATE — README requirements (subject chapter V)

### 24. Missing "Resources" section with AI usage description
- **File:** `README.md`
- **Subject:** "A 'Resources' section listing classic references … as well as a description of how AI was used — specifying for which tasks and which parts of the project"
- **Fix:** Add a `## Resources` section to `README.md`.

### 25. "Instructions" section label
- **File:** `README.md`
- **Subject:** Section must be named "Instructions" (not "Build & Run")
- **Fix:** Rename `## 🚀 Build & Run` to `## Instructions`.

---

## 🟢 BONUS (only after mandatory is complete)

- [ ] **Cookies and session management** — parse `Cookie` header, set `Set-Cookie` in responses, maintain a simple in-memory session store
- [ ] **Handle multiple CGI types** — e.g. both `.php` (php-cgi) and `.py` (python3)

---

## Summary table

| # | Area | Status |
|---|------|--------|
| 1 | Multiple servers / ports | ❌ Not done |
| 2 | Config wired to handlers | ❌ Not done |
| 3 | poll() read+write simultaneously | ❌ Not done |
| 4 | CGI execution | ❌ Not done |
| 5 | Chunked decoding | ❌ Not done |
| 6 | Allowed methods enforced | ❌ Not done |
| 7 | `root` from config | ❌ Not done |
| 8 | `index` from config | ❌ Not done |
| 9 | Redirections from config | ❌ Not done |
| 10 | `upload_path` from config | ❌ Not done |
| 11 | `client_max_body_size` enforced | ❌ Not done |
| 12 | Directory listing (autoindex) | ❌ Not done |
| 13 | Error pages from config | ❌ Not done |
| 14 | Missing default error pages | ❌ Incomplete |
| 15 | `Content-Length` in all responses | ❌ Not done |
| 16 | No errno after read/write | ⚠️ To verify |
| 17 | Default config path fallback | ❌ Not done |
| 18 | Partial send() handling | ❌ Not done |
| 19 | Request timeout | ❌ Not done |
| 20 | Multiple server fds in poll | ❌ Not done |
| 21 | Path validator decoupled from disk | ⚠️ To fix |
| 22 | Binary-safe body parsing | ⚠️ To fix |
| 23 | HTTP/1.0 compatibility | ⚠️ Optional |
| 24 | README Resources + AI usage | ❌ Missing |
| 25 | README "Instructions" label | ❌ Wrong label |
| B1 | Cookies / sessions | 🔵 Bonus |
| B2 | Multiple CGI types | 🔵 Bonus |
