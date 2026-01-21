/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 11:26:41 by aaiache           #+#    #+#             */
/*   Updated: 2026/01/21 15:27:50 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main()
{
    try
    {
        Server server;
        server.setup();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
