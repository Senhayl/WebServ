# webserv

## 🕸️ Description

`webserv` est un projet de l'école 42 dont l'objectif est de recréer un **serveur HTTP** en C++, inspiré du fonctionnement de **Nginx**. Le programme doit être capable de gérer des requêtes HTTP, de servir des fichiers statiques et d'exécuter des scripts CGI, tout en respectant les standards du protocole HTTP/1.1.

Ce projet met l'accent sur :

* la programmation système
* la gestion des sockets
* le multiplexage des connexions
* la rigueur et la robustesse du code

---

## 🎯 Objectifs du projet

* Comprendre le fonctionnement d'un serveur web
* Manipuler les sockets TCP/IP
* Implémenter le protocole HTTP/1.1
* Gérer plusieurs clients simultanément sans bloquer
* Parser un fichier de configuration
* Approfondir la programmation orientée objet en C++

---

## ⚙️ Fonctionnalités

### Requêtes HTTP

* Méthodes supportées :

  * `GET`
  * `POST`
  * `DELETE`

### Gestion du serveur

* Serveur non bloquant
* Multiplexage via `poll()` (ou équivalent)
* Gestion de plusieurs ports
* Support de plusieurs *server blocks*

### Configuration

* Fichier de configuration inspiré de Nginx
* Support des directives :

  * `listen`
  * `server_name`
  * `root`
  * `index`
  * `error_page`
  * `client_max_body_size`
  * `location`

### CGI

* Exécution de scripts CGI (ex: PHP, Python)
* Gestion des variables d'environnement
* Communication serveur ↔ CGI via pipes

### Autres fonctionnalités

* Upload de fichiers
* Pages d'erreur personnalisées
* Gestion correcte des codes de statut HTTP

---

## 🗂️ Structure du projet

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

## 🚀 Compilation et exécution

### Compilation

```bash
make
```

### Lancement du serveur

```bash
./webserv conf/default.conf
```

Si aucun fichier de configuration n'est fourni, un fichier par défaut est utilisé.

---

## 🌐 Utilisation

Une fois le serveur lancé, vous pouvez accéder au site via votre navigateur :

```
http://localhost:8080
```

Exemples de tests avec `curl` :

```bash
curl http://localhost:8080/
curl -X POST http://localhost:8080/upload
curl -X DELETE http://localhost:8080/file.txt
```

---

## 🧪 Tests

* Tests manuels via navigateur
* Tests avec `curl`
* Comparaison du comportement avec Nginx
* Vérification avec des requêtes invalides

---

## ⚠️ Contraintes

* C++98 uniquement
* Fonctions système autorisées uniquement
* Aucune fonction bloquante (`fork`, `execve` autorisés pour CGI)
* Un seul `poll()` (ou équivalent) pour gérer toutes les connexions

---

## 📚 Ressources utiles

* RFC 7230–7235 (HTTP/1.1)
* Documentation Nginx
* `man poll`, `man socket`, `man bind`

---

## 👥 Auteurs

Projet réalisé par :

* [Votre nom]
* [Nom du binôme]

---

## ✅ Statut

🚧 En cours de développement
