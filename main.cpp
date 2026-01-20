/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 11:26:41 by aaiache           #+#    #+#             */
/*   Updated: 2026/01/20 16:14:06 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET = ipv4, SOCK_STREAM = TCP, 0 = protocole pardefaut TCP
    if (server_fd < 0)
        return 1;
    sockaddr_in addr; //specifique a ipv4, va servir a bind.
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; //sin_family = type d'adresse
    addr.sin_port = htons(8080); //mettre le port dans le dossier de configutarion, sin_port = port.
    addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY = localhost, toutes interfaces.

    bind(server_fd, (sockaddr*)&addr, sizeof(addr)); //socket/adresse ipv4 castee/ taille de la struct
    listen(server_fd, 10);//passe le socket en mode serveur, creer une file dattente de connexions

    std::vector<pollfd> fds; //creer un vector de pollfd (fds, events = events attendus, revents = evenement detectes)
    pollfd server_poll;
    server_poll.fd = server_fd; //s-e
    server_poll.events = POLLIN;// donnees en attente de lecture
    fds.push_back(server_poll);

    std::cout << "Server listening on port " << "8080\n";

    while (true)
    {
        poll(&fds[0], fds.size(), -1);//-1 = delai, poll = attendre un evenement concernant ce fd.

        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                // Nouveau client
                if (fds[i].fd == server_fd)
                {
                    int client_fd = accept(server_fd, NULL, NULL);
                    pollfd client_poll;
                    client_poll.fd = client_fd;
                    client_poll.events = POLLIN;
                    fds.push_back(client_poll);
                    std::cout << "New client connected\n";
                    std::string response =
                     "HTTP/1.1 200 OK\r\n"
                    "Content-Length: 13\r\n"
                    "Content-Type: text/plain\r\n"
                    "\r\n"
                    "Hello, world";
                    send(fds[i].fd, response.c_str(), response.size(), 0);
                }
                else
                {
                    char buffer[1024];
                    int bytes = recv(fds[i].fd, buffer, 1024, 0);
                    if (bytes <= 0)
                    {
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;
                        std::cout << "Client disconnected\n";
                    }
                    else
                    {
                        buffer[bytes] = 0;
                        std::cout << "Received:\n" << buffer << std::endl;
                    }
                }
            }
        }
    }
}
