/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:12:18 by yzaytoun          #+#    #+#             */
/*   Updated: 2025/03/29 19:06:59 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

# ifndef WEBSERV_HOME
#  define WEBSERV_HOME "/"
# endif

#include <string>
#include <climits>

# define ONE_KILOBYTE	1024
# define ONE_MEGABYTE	1024 * 1024
# define ONE_GIGABYTE 	1024 * 1024 * 1024

/* Defualt Values */
# define DOCROOT      	        "/Docroot"
# define DEF_LOCATION_PATH		"/"
# define DEF_ROOT				"/html"
# define DEF_ERROR_ROOT			"/error_pages"
# define DEF_DELETE_SUCCESS		"/deleted.html"
# define DEF_SUCCESS_PAGE		"/success.html"
# define DEF_CGI_ROOT			"/cgi-bin"
# define DEF_UPLOADS_ROOT		"/uploads"
# define DEF_CLIENT_BODY		ONE_MEGABYTE
# define DEF_PORT				"8080"
# define DEF_HOST				"127.0.0.1"
# define DEF_INDEX				"/index.html"
# define DEF_DIR_PERMISSIONS	S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH
# define STRING_NULL            std::string::npos

/* Default Timeouts */
# define DEF_TIMEOUT			30
# define RECV_TIMEOUT_SEC		1
# define RECV_TIMEOUT_USEC		1000
# define SEND_TIMEOUT_SEC		1
# define SEND_TIMEOUT_USEC		1000

/* Default Error Pages */
# define DEF_404				"/404.html"	
# define DEF_50X				"/50x.html"	
# define DEF_ERROR_PAGE			"/error.html"	

/* Epoll Constants */
# define EPOLL_MAX_EVENTS		5
# define EPOLL_READ_SIZE		10
# define EPOLL_WAIT_TIME_MS 	1000

/* Print Colores */
# define RED		"\033[31;1;3m"
# define GREEN		"\033[32;1;3m"
# define YELLOW		"\033[33;1;3m"
# define MAGENTA	"\033[35;1;3m"
# define CYAN		"\033[36;1;3m"
# define RESET		"\033[0m"



#endif	/* MACROS_H */