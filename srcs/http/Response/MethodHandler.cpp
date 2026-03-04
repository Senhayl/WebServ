#include "MethodHandler.hpp"
#include <sys/stat.h>
#include <dirent.h>

static std::string generateAutoindex(const std::string& dirPath, const std::string& uri) {
	DIR* dir = opendir(dirPath.c_str());
	if (!dir)
		return "";

	std::ostringstream html;
	html << "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">"
		 << "<title>Index of " << uri << "</title></head><body>"
		 << "<h1>Index of " << uri << "</h1><hr><ul>";

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		std::string name = entry->d_name;
		if (name == ".")
			continue;
		std::string href = uri;
		if (href.empty() || href[href.size() - 1] != '/')
			href += '/';
		href += name;
		if (entry->d_type == DT_DIR)
			name += "/";
		html << "<li><a href=\"" << href << "\">" << name << "</a></li>";
	}
	closedir(dir);
	html << "</ul><hr></body></html>";
	return html.str();
}

std::string getContentType(const std::string& path) {
	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos)
		return "application/octet-stream";
	
	std::string ext = path.substr(pos + 1);
	if (ext == "html" || ext == "htm") return "text/html";
	if (ext == "css") return "text/css";
	if (ext == "js") return "application/js";
	if (ext == "json") return "application/json";
	if (ext == "xml") return "application/xml";
	if (ext == "txt") return "text/plain";
	if (ext == "png") return "image/png";
	if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
	if (ext == "gif") return "image/gif";
	if (ext == "svg") return "image/svg+xml";
	if (ext == "pdf") return "application/pdf";
	if (ext == "zip") return "application/zip";
	
	return "application/octet-stream";
}


HttpResponse MethodHandler::handlerGET(const HttpRequest& req, const ServerConfig& server, const Location& loc) {
	std::string reqPath = req.getPath();
	if (reqPath == "/") {
		if (loc.getIndex().empty())
			reqPath = "/" + server.getIndex();
		else
			reqPath = "/" + loc.getIndex();
	}
	if (reqPath == "/redirections.html") {
		HttpResponse resp(301);
		resp.setStatusMessage(301);
		resp.addHeader("Location", "https://youtu.be/DQJPt2GdoZM");
		resp.addHeader("Content-Length", "0");
		resp.addHeader("Connection", "close");
		return resp;
	}

	std::string path;
	if (loc.getRoot().empty())
		path = server.getRoot();
	else
		path = loc.getRoot();

	path += reqPath;

	struct stat st;
	if (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
		if (loc.getAutoindex()) {
			std::string listing = generateAutoindex(path, reqPath);
			if (listing.empty())
				return HttpResponse::createError(500);
			std::string ct = "text/html";
			return HttpResponse::createResponse(200, listing, ct);
		}
		return HttpResponse::createError(403);
	}

	std::ifstream file(path.c_str());
	if (!file.is_open())
		return HttpResponse::createError(404);
	
	std::ostringstream content;
	content << file.rdbuf();
	file.close();

	std::string body = content.str();
	std::string contentType = getContentType(reqPath);
	return HttpResponse::createResponse(200, body, contentType);
}

HttpResponse MethodHandler::handlerPOST(const HttpRequest& req, const ServerConfig& server, const Location& loc) {
	(void)server;
	if (req.getBody().empty()) {
		return HttpResponse::createError(400);
	}
	if (loc.getUploadPath().empty()) {
		return HttpResponse::createError(403);
	}
	std::string uri = req.getPath();
	std::string filename = uri.substr(uri.find_last_of('/'));
	std::string path = loc.getUploadPath() + filename;
	
	std::ofstream file(path.c_str(), std::ios::out);
	if (!file.is_open()) {
		return HttpResponse::createError(500);
	}
	file.write(req.getBody().c_str(), req.getBody().size());
	file.close();
	
	HttpResponse resp(201);
	resp.setStatusMessage(201);
	resp.addHeader("Location", req.getPath());
	resp.addHeader("Connection", "close");
	return resp;
}

HttpResponse MethodHandler::handlerDELETE(const HttpRequest& req, const ServerConfig& server, const Location& loc) {
	std::string path;
	if (loc.getRoot().empty())
		path = server.getRoot();
	else
		path = loc.getRoot();
	path += req.getPath();

	std::ifstream file(path.c_str());
	if (!file.is_open()) {
		return HttpResponse::createError(404);
	}
	file.close();
	if (remove(path.c_str()) != 0) {
		return HttpResponse::createError(500);
	}

	HttpResponse resp(204);
	resp.setStatusMessage(204);
	return resp;
}