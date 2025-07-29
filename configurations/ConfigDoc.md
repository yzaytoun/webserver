# Configuration File 
This document resumes the fields to be managed by webserv according to the subject

__interesting links__:
- [How nginx process a request?](https://nginx.org/en/docs/http/request_processing.html)
- [Nginx Core functionality](https://nginx.org/en/docs/ngx_core_module.html)
- [__Nginx Http Core Module__](https://nginx.org/en/docs/http/ngx_http_core_module.html)
- [__Nginx Development Guide__](https://nginx.org/en/docs/dev/development_guide.html)
- [__Module ngx_http_api_module__](https://nginx.org/en/docs/http/ngx_http_api_module.html)

--- 

## Scopes in configuration file:
- http
- Server
- Location


## Fields in configuration file:
| Field | Description | Syntax| Example| Default|
| ----------- | ----------- | ---------- | --------- | ------ |
| listen | Sets the address and port for IP, or the path for a UNIX-domain socket on which the server will accept requests. Both address and port, or only address or only port can be specified. An address may also be a hostname | ```listen address[:port] [default_server]```<br> ```listen port [default_server]``` | listen 127.0.0.1;<br>listen 8000;<br>listen *:8000;<br>listen localhost:8000;| listen *:80 \| *:8000; |
| server_name | Sets names of a virtual server, the first name becomes the primary server name. | ```server_name name ...;```|  server_name example.com \*.example.com www.example.\*; | server_name ""; |
| set | Sets a value for the specified variable. The value can contain text, variables, and their combination. | ```set $variable value;```|  set $base /home/webserv;| - |
| return | Stops processing and returns the specified code to a client. The non-standard code 444 closes a connection without sending a response header. | ```return code [text];```<br> ```return code URL;```<br> ```return URL;```|  return 403;| - |
| root | Sets the root directory for requests | ```root path;```| location /i/ {<br>root /data/w3; <br>}| html |
| error_page | Defines the URI that will be shown for the specified errors. A uri value can contain variables. | ```	error_page code ... [=[response]] uri;```| error_page 404             /404.html; <br> error_page 500 502 503 504 /50x.html; <br><br> location / {error_page 404 = @fallback;} <br> error_page 404 =200 /empty.gif;| - |
| include | Includes another file, or files matching the specified mask, into configuration. Included files should consist of syntactically correct directives and blocks. | ```include file \| mask;``` | include mime.types; <br> include vhosts/*.conf;| - |
| add_header | Adds the specified field to a response header provided that the response code equals 200, 201, 204, 206, 301, 302, 303, 304, 307, 308. Parameter value can contain variables.The "Allow" header field lists the set of methods advertised as supported by the target resource. The purpose of this field is strictly to inform the recipient of valid request methods associated with the resource. [Http RFC](https://datatracker.ietf.org/doc/html/rfc9110#field.allow) |  ```add_header name value [always];``` <br><br>```add_header Allow "GET, POST, HEAD" always;``` <br> ```if (\$request\_method !~ ^(GET\|POST\|HEAD)$ ) {return 405;}``` <br><br> ```if (\$request\_method !~ ^(GET\|POST\|PUT\|PATCH)$){return'404';}``` <br> ```allow_methods GET POST PUT HEAD```| allow_methods GET POST PUT HEAD; <br> add_header Allow: GET, POST, HEAD | - |
| limit_except | Limits allowed HTTP methods inside a location. The method parameter can be one of the following: GET, HEAD, POST, PUT, DELETE, MKCOL, COPY, MOVE, OPTIONS, PROPFIND, PROPPATCH, LOCK, UNLOCK, or PATCH. Allowing the GET method makes the HEAD method also allowed | ```limit_except method ... { ... }``` <br> ```limit_except METHOD1 [METHOD2…] {  allow \| deny \| auth_basic \| auth_basic_user_file \| proxy_pass  perl; }```| limit_except GET {<br> allow 192.168.1.0/32; <br> deny  all; <br>}| - | 
| error_log | Configures logging. Several logs can be specified on the same configuration level (1.5.2). If on the main configuration level writing a log to a file is not explicitly defined, the default file will be used. | ```error_log file [level];```| error_log logs/error.log error; | - | 
| location | Sets configuration depending on a request URI. A location can either be defined by a prefix string, or by a regular expression. Regular expressions are specified with the preceding “~*” modifier (for case-insensitive matching), or the “~” modifier (for case-sensitive matching). To find location matching a given request, nginx first checks locations defined using the prefix strings (prefix locations). Among them, the location with the longest matching prefix is selected and remembered. Then regular expressions are checked, in the order of their appearance in the configuration file. The search of regular expressions terminates on the first match, and the corresponding configuration is used. If no match with a regular expression is found then the configuration of the prefix location remembered earlier is used. __location blocks can be nested, with some exceptions.__| ```location [ = \| ~ \| ~* \| ^~ ] uri { ... }``` <br> ```location @name { ... }```| | - |
| try_files | Checks the existence of files in the specified order and uses the first found file for request processing; the processing is performed in the current context. The path to a file is constructed from the file parameter according to the root and alias directives. It is possible to check directory’s existence by specifying a slash at the end of a name, e.g. “$uri/”. If none of the files were found, an internal redirect to the uri specified in the last parameter is made. | ```try_files file ... uri;``` <br> ```try_files file ... =code;```| location /images/ {<br> try_files $uri /images/default.gif;<br> }| - |
| client_max_body_size | Sets the maximum allowed size of the client request body. If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client. Please be aware that browsers cannot correctly display this error. Setting size to 0 disables checking of client request body size. | ```client_max_body_size size;```| client_max_body_size 1m;|
| auto_index | Enables or disables the directory listing output. The ngx_http_autoindex_module module processes requests ending with the slash character (‘/’) and produces a directory listing. Usually a request is passed to the ngx_http_autoindex_module module when the ngx_http_index_module module cannot find an index file. | ```autoindex off;```| location / {<br>autoindex on;<br>}| off |

## Example .config
```
server
{ 
    listen      localhost:80;
    listen      [::]:80;
    server_name www.example.com;
    set         $base /home/webserv;
    root        $base/public;

    # security
    include     nginxconfig.io/security.conf;

    # restrict methods
     add_header Allow "GET, POST, HEAD" always;
     if ( $request_method !~ ^(GET|POST|HEAD)$ ) {
        return 405 http://localhost:5000;
     }

    # logging
    access_log /var/log/nginx/access.log combined buffer=512k flush=1m;
    error_log  /var/log/nginx/error.log warn;

    # index.html fallback
    location / {
        try_files $uri $uri/ /index.html;
    }

    # index.php fallback
    location ~ ^/api/ {
        try_files $uri $uri/ /index.php?$query_string;
    }

    # additional config
    include nginxconfig.io/general.conf;
    include nginxconfig.io/example.com.wordpress.conf;

    # handle .php
    location ~ \.php$ {
         fastcgi_pass  localhost:9000;
        fastcgi_param SCRIPT_FILENAME
                      $document_root$fastcgi_script_name;
        include       fastcgi_params;
    }
}
```