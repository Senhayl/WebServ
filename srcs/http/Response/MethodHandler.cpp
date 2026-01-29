#include "MethodHandler.hpp"


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


HttpResponse MethodHandler::handlerGET(const HttpRequest& req) {
	std::string path = "../Pages/";
	path += req.getPath();
	std::ifstream file(path.c_str());
	if (!file.is_open())
		return HttpResponse::createError(404);
	
	std::ostringstream content;
	content << file.rdbuf();
	file.close();

	std::string body = content.str();
	std::string contentType = getContentType(req.getPath());
	return HttpResponse::createResponse(200, body, contentType);
}

HttpResponse MethodHandler::handlerPOST(const HttpRequest& req) {
	if (req.getBody().empty()) {
		return HttpResponse::createError(400);
	}

	std::string path = "../Pages/";
	path += req.getPath();
	
	std::ofstream file(path.c_str(), std::ios::binary);
	if (!file.is_open()) {
		return HttpResponse::createError(500);
	}
	file.write(req.getBody().c_str(), req.getBody().size());
    file.close();
	
	HttpResponse resp(201);
    resp.setStatusMessage(201);
    resp.addHeader("Location", req.getPath());
	return resp;
}

HttpResponse MethodHandler::handlerDELETE(const HttpRequest& req) {
	std::string path = "../Pages/";
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