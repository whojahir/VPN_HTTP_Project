# VPN_HTTP_Project

A lightweight HTTP/1.1 server and client implemented from scratch in C using raw BSD sockets — built as the foundational networking layer for a future VPN tunneling project.

## Overview

This project demonstrates low-level network programming without relying on any HTTP framework or library. The server handles raw TCP connections, parses HTTP requests manually, and serves static files with proper MIME types and headers. It is the first phase of a broader project exploring VPN tunneling concepts.

## Features

- Custom HTTP/1.1 server built on raw TCP sockets
- Handles `GET` requests and serves static files from a `www/` directory
- Automatic `Content-Type` and `Content-Length` header generation
- Graceful `404 Not Found` handling for missing resources
- A minimal HTTP client for testing requests against the server
- Clean separation between server logic (`http_server.c`) and client logic (`http_client.c`)
