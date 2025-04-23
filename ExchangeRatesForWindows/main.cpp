#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <cstdio>
#include <string>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <istream>
#include <fstream>
#include <sstream> 
#include <chrono>
#include <thread>
#include <sql.h>

#include "ExchangeRates.h"

std::unordered_map<std::string, Valute> getExchangeRates()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	sockaddr_in sockAddr;
	struct hostent* host = nullptr;
	std::vector<char> workBuffer(10000);
	std::string source;
	std::vector<std::string> strings;
	std::unordered_map<std::string, Valute> fieldValutes;
	std::string xmlText;

	auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result == WSASYSNOTREADY)
	{
		std::string text("ERROR: The underlying network subsystem is not ready for network data exchange. ");
		text += std::to_string(WSAGetLastError());
		throw std::runtime_error(text);
	}
	else if (result == WSAVERNOTSUPPORTED)
	{
		std::string text("ERROR: The requested version of Windows sockets support is not");
		text += std::to_string(WSAGetLastError());
		throw std::runtime_error(text);
	}
	else if (result == WSAEINPROGRESS)
	{
		std::string text("ERROR: Windows 1.1 socket blocking is in progress. ");
		text += std::to_string(WSAGetLastError());
		throw std::runtime_error(text);
	}
	else if (result == WSAEPROCLIM)
	{
		std::string text("ERROR: The limit on the number of tasks supported by the Windows sockets implementation has been reached. ");
		text += std::to_string(WSAGetLastError());
		throw std::runtime_error(text);
	}
	else if (result == WSAEFAULT)
	{
		std::string text("ERROR: The lpWSAData parameter is not a valid pointer. ");
		text += std::to_string(WSAGetLastError());
		throw std::runtime_error(text);
	}
	else if (result != 0)
	{
		std::string text("ERROR: WSA  initialization error. ");
		text += std::to_string(WSAGetLastError());
		throw std::runtime_error(text);
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::string text("ERROR: The socket is not initialized. ");
		text += std::to_string(WSAGetLastError());
		WSACleanup();
		throw std::runtime_error(text);
	}
	else
	{
		std::cout << "INFO: The socket was initialized successfully. " << std::endl;
	}

	host = gethostbyname(URL.c_str());
	if (host == nullptr)
	{
		std::string text("ERROR: gethostbyname return nullptr. ");
		text += std::to_string(WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		throw std::runtime_error(text);
	}
	else
	{
		std::cout << "INFO: gethostbyname completed successfully. " << std::endl;
	}

	sockAddr.sin_port = htons(80);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = *((unsigned long*)host->h_addr);

	result = connect(clientSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));

	if (result == SOCKET_ERROR)
	{
		const std::string text = "ERROR: connection failed. " + std::to_string(WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		throw std::runtime_error(text);
	}
	else
	{
		std::cout << "INFO: connection established successfully." << std::endl;
	}

	std::string GET = "";
	GET += "GET " + QUERY + " HTTP/1.1\r\n";
	//GET += "Accept-Encoding: deflate, gzip\r\n";
	GET += "Accept-Language: ru, en\r\n";
	GET += "Cache-Control:  no-cache\r\n";
	GET += "Content-Type:  application/xml; charset=utf-8\r\n";
	//GET += "Pragma: no-cache\r\n";
	GET += "Host: " + URL + "\r\n";
	//GET += "Host: " + URL + "\r\n\r\n";
	GET += "Connection: close\r\n\r\n";

	result = send(clientSocket, GET.c_str(), strlen(GET.c_str()), NULL);
	if (result == WSANOTINITIALISED)
	{
		std::cout << "ERROR: Before this function can be used, a successful call to WSAStartup must occur. "
			<< WSAGetLastError() << std::endl;
	}
	else if (result == WSAENETDOWN)
	{
		std::cout << "ERROR: A network subsystem failure occurred. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAEACCES)
	{
		std::cout << "ERROR: The requested address is a broadcast address, but the corresponding flag is not set. Call setsockopt with the SO_BROADCAST socket option to enable the use of the broadcast address.The requested address is a broadcast address, but the corresponding flag is not set. Call setsockopt with the SO_BROADCAST socket option to enable the use of the broadcast address. " <<
			WSAGetLastError() << std::endl;
	}
	else if (result == WSAEINTR)
	{
		std::cout << "ERROR: A Windows Sockets 1.1 blocking call was cancelled via WSACancelBlockingCall. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAEINPROGRESS)
	{
		std::cout << "ERROR: A blocking Windows Sockets 1.1 call is in progress or the service provider is still handling the callback function. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAEFAULT)
	{
		std::cout << "ERROR: The buf parameter is not entirely contained within the valid portion of the user's address space. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAENETRESET)
	{
		std::cout << "ERROR: The connection was terminated due to an error during the operation. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAENOBUFS)
	{
		std::cout << "ERROR: There is no free space in the buffer. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAENOTCONN)
	{
		std::cout << "ERROR: The socket is not connected. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAENOTSOCK)
	{
		std::cout << "ERROR: The descriptor is not a socket. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAEOPNOTSUPP)
	{
		std::cout << "ERROR: MSG_OOB is specified but the socket is not a stream socket, such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations. " <<
			WSAGetLastError() << std::endl;
	}
	else if (result == WSAESHUTDOWN)
	{
		std::cout << "ERROR: The socket was shut down; it is not possible to send to the socket after calling shutdown with the SD_SEND or SD_BOTH option. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAEWOULDBLOCK)
	{
		std::cout << "ERROR: The socket is marked as non-blocking and the requested operation will block. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAEMSGSIZE)
	{
		std::cout << "ERROR: The socket is message-oriented, and the message is larger than the maximum supported by the underlying transport. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAEHOSTUNREACH)
	{
		std::cout << "ERROR: This node is currently unable to contact the remote node. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAEINVAL)
	{
		std::cout << "ERROR: The socket was not bound using bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAECONNABORTED)
	{
		std::cout << "ERROR: The virtual connection was broken due to a timeout or other failure. The application should close the socket because it can no longer be used. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAECONNRESET)
	{
		std::cout << "ERROR: The virtual connection was reset by the remote end via an abort. For UDP sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with an ICMP Port unreachable packet. The application should close the socket because it is no longer usable. " << WSAGetLastError() << std::endl;
	}
	else if (result == WSAETIMEDOUT)
	{
		std::cout << "ERROR: The connection was interrupted due to a network failure or because the system at the other end was down without prior notice. " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "INFO: Total number of bytes sent " << result << std::endl;
	}

	int recResult = 0;

	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "INVALID SOCKET" << std::endl;
	}
	else
	{
		std::cout << "NOT INVALID SOCKET" << std::endl;
	}

	do {
		recResult = recv(clientSocket, workBuffer.data(), workBuffer.size(), MSG_WAITALL);
		if (recResult > 0)
		{
			printf("Bytes received: %d\n", recResult);
			source += std::string(workBuffer.data(), workBuffer.size());
		}
		else if (recResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (recResult > 0);

	int startIndex{ 0 };
	int endIndex{ 0 };
	int currentIndex{ 0 };
	int count{ 0 };

	while (currentIndex < source.size())
	{
		startIndex = currentIndex;
		while (source[currentIndex] != '\r' && source[currentIndex + 1] != '\n' &&
			currentIndex + 1 < source.size())
		{
			++currentIndex;
		}
		endIndex = currentIndex;

		const auto strLength = endIndex - startIndex + 1;
		strings.push_back(std::string(&source[startIndex], strLength));

		currentIndex += 2;
	}

	auto is_empty = [](const std::string& line) -> bool
		{
			auto isEmpty = true;
			for (int idx = 0; idx < line.size(); ++idx)
			{
				if (line[idx] != ' ' && line[idx] != '\r' && line[idx] != '\n')
				{
					isEmpty = false;
				}
			}
			return isEmpty;
		};

	for (int idx = 0; idx < strings.size(); ++idx)
	{
		if (is_empty(strings[idx]))
		{
			xmlText = strings[idx + 1];
			break;
		}
	}

	auto clear_text = [](const std::string& text) -> std::string
		{
			std::vector<char> buffer;
			for (int idx = 0; idx < text.size(); ++idx)
			{
				if (text[idx] == 1 && text[idx + 1] == 1 && text[idx + 2] == 1)
				{
					break;
				}
				buffer.push_back(text[idx]);
			}
			buffer.push_back('\0');
			buffer.shrink_to_fit();
			return std::string(buffer.data(), buffer.size());
		};

	xmlText = clear_text(xmlText);

	using namespace boost::property_tree::detail::rapidxml;
	xml_document<> doc;
	xml_node<>* root_node;
	std::vector<char> buffer(xmlText.begin(), xmlText.end());
	buffer.shrink_to_fit();
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("ValCurs");

	using pointer = xml_node<>*;

	Valute valute;

	for (pointer valute_node = root_node->first_node("Valute"); valute_node; valute_node = valute_node->next_sibling())
	{
		valute.ID = std::string(valute_node->first_attribute("ID")->value(), valute_node->first_attribute("ID")->value_size());
		//std::cout << "ID = " << valute.ID << " ";

		pointer num_code = valute_node->first_node("NumCode");
		valute.NumCode = std::stoi(std::string(num_code->value(), num_code->value_size()));
		//std::cout << "NumCode = " << num_code->value() << " ";

		pointer char_node = valute_node->first_node("CharCode");
		valute.CharCode = std::string(char_node->value(), char_node->value_size());
		//std::cout << "CharCode = " << char_node->value() << " ";

		pointer nominal_node = valute_node->first_node("Nominal");
		valute.Nominal = std::stoi(std::string(nominal_node->value(), nominal_node->value_size()));
		//std::cout << "Nominal = " << nominal_node->value() << " ";

		pointer name_node = valute_node->first_node("Name");
		valute.Name = std::string(name_node->value(), name_node->value_size());
		//std::cout << "Name = " << name_node->value() << " ";

		pointer value_node = valute_node->first_node("Value");
		valute.Value = std::stof(std::string(value_node->value(), value_node->value_size()));
		//std::cout << "Value = " << value_node->value() << " ";

		pointer vunit_rate_node = valute_node->first_node("VunitRate");
		valute.VunitRate = std::stof(std::string(vunit_rate_node->value(), vunit_rate_node->value_size()));
		//std::cout << "VunitRate = " << vunit_rate_node->value() << " ";

		fieldValutes.insert(std::pair<std::string, Valute>(valute.CharCode, valute));

		//std::cout << "\n";
	}

	closesocket(clientSocket);
	WSACleanup();

	return fieldValutes;
}

void googleRequest()
{
	const std::string url = "www.yandex.ru";
	const std::string query = "анатолий+шарий+телеграм";

	WSAData wsaData;
	SOCKET Socket = INVALID_SOCKET;
	sockaddr_in sockAddr;
	struct hostent* host = nullptr;

	std::string GET = "";
	GET += "GET /search/?text=" + query + "&clid=2411725&search_source=dzen_desktop_safe&src=suggest_Pers&lr=192 HTTP/1.1\r\n";
	//GET += "Accept-Encoding: gzip, deflate, br";
	//GET += "Accept-Language: en-US, en; q=0.9, nb; q=0.1";
	GET += "Cache-Control: no-cache";
	GET += "Content-Type: text/plain; charset=UTF-8";
	GET += "Host: " + url + "\r\n";
	GET += "Connection: close\r\n";
	//GET += "Connection: keep-alive Keep-Alive\r\n";
	//GET += "Keep-Alive: timeout=5, max=100\r\n";

	auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	host = gethostbyname(url.c_str());

	sockAddr.sin_port = htons(80);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	result = connect(Socket, (SOCKADDR*)(&sockAddr), sizeof(sockAddr));
	if (result == SOCKET_ERROR)
	{
		const std::string text = "ERROR: connection failed. " + std::to_string(WSAGetLastError());
		closesocket(Socket);
		WSACleanup();
		throw std::runtime_error(text);
	}
	else
	{
		std::cout << "INFO: connection established successfully." << std::endl;
	}

	result = send(Socket, GET.c_str(), strlen(GET.c_str()), NULL);
	std::cout << "INFO: send = " << result << std::endl;


	std::vector<char> workBuffer(10000);
	std::string source;

	int recResult = 0;

	do {
		recResult = recv(Socket, workBuffer.data(), workBuffer.size(), MSG_WAITALL);
		if (recResult > 0)
		{
			printf("Bytes received: %d\n", recResult);
			source += std::string(workBuffer.data(), workBuffer.size());
		}
		else if (recResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (recResult > 0);

	std::cout << "\n";
	std::cout << "\n";
	std::cout << source << std::endl;
	std::cout << "\n";
	std::cout << "\n";

}

int error(std::string message)
{
	std::cout << "ERROR: " << message << std::endl;
	return -1;
}

int nonBlockingHTTPRequest()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	sockaddr_in sockAddr;
	struct hostent* host = nullptr;

	std::string HEAD = "";
	HEAD += "HEAD " + QUERY + " HTTP/1.1\r\n";
	//GET += "Accept-Encoding: deflate, gzip\r\n";
	HEAD += "Accept-Language: ru, en\r\n";
	HEAD += "Cache-Control:  no-cache\r\n";
	HEAD += "Content-Type:  application/xml; charset=utf-8\r\n";
	//GET += "Pragma: no-cache\r\n";
	//HEAD += "Host: " + URL + "\r\n";
	HEAD += "Host: " + URL + "\r\n\r\n";
	//GET += "Connection: close\r\n\r\n";

	std::string GET_keep = "";
	GET_keep += "GET " + QUERY + " HTTP/1.1\r\n";
	//GET += "Accept-Encoding: deflate, gzip\r\n";
	GET_keep += "Accept-Language: ru, en\r\n";
	GET_keep += "Cache-Control:  no-cache\r\n";
	GET_keep += "Content-Type:  application/xml; charset=utf-8\r\n";
	GET_keep += "Pragma: no-cache\r\n";
	//GET_keep += "Host: " + URL + "\r\n";
	GET_keep += "Host: " + URL + "\r\n\r\n";

	std::string GET = "";
	GET += "GET " + QUERY + " HTTP/1.1\r\n";
	//GET += "Accept-Encoding: deflate, gzip\r\n";
	GET += "Accept-Language: ru, en\r\n";
	GET += "Cache-Control:  no-cache\r\n";
	GET += "Content-Type:  application/xml; charset=utf-8\r\n";
	//GET += "Pragma: no-cache\r\n";
	GET += "Host: " + URL + "\r\n";
	//GET += "Host: " + URL + "\r\n\r\n";
	GET += "Connection: close\r\n\r\n";

	auto send_fun = [&clientSocket](const std::string& httpQuery)
		{
			std::cout << "send_fun()" << std::endl;
			fd_set	write_s;
			timeval	time_out;
			time_out.tv_sec = 0;
			time_out.tv_usec = 500000; // 0.5 sec.
			auto result = 0;

			FD_ZERO(&write_s);	 // Обнуляем мнодество
			FD_SET(clientSocket, &write_s); // Заносим в него наш сокет 
			result = select(0, NULL, &write_s, NULL, &time_out);
			if (result == SOCKET_ERROR)
			{
				std::cout << "ERROR: select() failed " << WSAGetLastError() << std::endl;
				closesocket(clientSocket);
				WSACleanup();
				return;
			}
			else
			{
				std::cout << "INFO: select() success " << std::endl;
			}

			if ((result != 0) && (FD_ISSET(clientSocket, &write_s)))
			{
				result = send(clientSocket, httpQuery.c_str(), httpQuery.size(), NULL);
				if (result > 0)
				{
					std::cout << "INFO: send() = " << result << std::endl;
				}
				else
				{
					std::cout << "ERROR: send() failed " << WSAGetLastError() << std::endl;
				}
			}
		};

	auto recv_fun = [&clientSocket]() -> std::string
		{
			std::cout << "recv_fun()" << std::endl;
			int result = 0;
			std::vector<char> workBuffer(10000);
			std::string source;
			fd_set	read_s;
			timeval	time_out;
			time_out.tv_sec = 0;
			time_out.tv_usec = 500000; // 0.5 sec.
			int count = 0;
			int size = 0;

			do
			{
				FD_ZERO(&read_s);	 // Обнуляем мнодество
				FD_SET(clientSocket, &read_s); // Заносим в него наш сокет 
				result = select(0, &read_s, NULL, NULL, &time_out);
				if (result == SOCKET_ERROR)
				{
					std::cout << "ERROR: select() failed " << WSAGetLastError() << std::endl;
					closesocket(clientSocket);
					WSACleanup();
					return std::string();
				}
				else
				{
					std::cout << "INFO: select() success " << std::endl;
				}
				if ((result != 0) && (FD_ISSET(clientSocket, &read_s)))
				{
					result = recv(clientSocket, workBuffer.data(), workBuffer.size(), 0);
					if (result == SOCKET_ERROR)
					{
						std::cout << "ERROR: recv() failed " << WSAGetLastError() << std::endl;
						closesocket(clientSocket);
						WSACleanup();
						return std::string();
					}
					else if (result == 0)
					{
						std::cout << "Connection close" << std::endl;
					}
					else if (result > 0)
					{
						source += std::string(workBuffer.data(), workBuffer.size());
						std::cout << "INFO: recv() = " << result << ", count = " << count << std::endl;
						++count;
						size += result;
						std::cout << "INFO: size = " << size << std::endl;
					}
				}

			} while (result > 0);

			return source;
		};

	auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result == 0)
	{
		std::cout << "INFO: WSAStartup() success " << std::endl;
	}
	else
	{
		std::cout << "ERROR: WSAStartup() failed " << WSAGetLastError() << std::endl;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "ERROR: socket() failed " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "INFO: socket() success" << std::endl;
	}

	host = gethostbyname(URL.c_str());
	if (host == nullptr)
	{
		std::string text("ERROR: gethostbyname return nullptr. ");
		text += std::to_string(WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		throw std::runtime_error(text);
	}
	else
	{
		std::cout << "INFO: gethostbyname completed successfully. " << std::endl;
	}

	sockAddr.sin_port = htons(80);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = *((unsigned long*)host->h_addr);

	result = connect(clientSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
	if (result == SOCKET_ERROR)
	{
		const std::string text = "ERROR: connection failed. " + std::to_string(WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		throw std::runtime_error(text);
	}
	else
	{
		std::cout << "INFO: connection established successfully." << std::endl;
	}

	const unsigned long l = 1;
	result = ioctlsocket(clientSocket, FIONBIO, (unsigned long*)&l);
	if (result == SOCKET_ERROR)
	{
		std::cout << "ERROR: ioctlsocket() failed " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	else
	{
		std::cout << "INFO: ioctlsocket() success " << std::endl;
	}

	send_fun(HEAD);
	const auto source_1 = recv_fun();
	send_fun(GET_keep);
	const auto source_keep = recv_fun();
	send_fun(GET);
	const auto source_2 = recv_fun();


	std::cout << "\n";
	std::cout << "INFO: SOURCE_1" << std::endl;
	std::cout << source_1 << std::endl;
	std::cout << "\n";

	std::cout << "\n";
	std::cout << "INFO: SOURCE_KEEP" << std::endl;
	std::cout << source_keep << std::endl;
	std::cout << "\n";

	std::cout << "\n";
	std::cout << "INFO: SOURCE_2" << std::endl;
	std::cout << source_2 << std::endl;
	std::cout << "\n";

	closesocket(clientSocket);
	WSACleanup();
	return 0;
}


int main()
{ }