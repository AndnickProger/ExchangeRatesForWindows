#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <cstdio>
#include <string>
#include <chrono>
#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <istream>
#include <fstream>

#include <boost/property_tree/detail/rapidxml.hpp>
#include <boost/property_tree/detail/xml_parser_read_rapidxml.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/config.hpp>

#pragma comment(lib, "Ws2_32.lib") 

static const std::string URL = "www.cbr.ru";
static const std::string QUERY = "/scripts/XML_daily.asp?";

struct Date
{
	int day;
	int month;
	int year;
};

struct Valute
{
	std::string ID;
	int NumCode;
	std::string CharCode;
	int Nominal;
	std::string Name;
	float Value;
	float VunitRate;
};

class ExchangeRates
{
public:

	using valute_type = std::unordered_map<std::string, Valute>;
	using reference = std::unordered_map<std::string, Valute>&;
	using const_reference = const std::unordered_map<std::string, Valute>&;
	using data_type = Date;
	using date_reference = Date&;
	using date_const_reference = const Date&;

	ExchangeRates();

	ExchangeRates(const Date& date);

	ExchangeRates(const ExchangeRates& other);

	ExchangeRates& operator = (const ExchangeRates& other);

	ExchangeRates(ExchangeRates&& other) noexcept;

	ExchangeRates& operator = (ExchangeRates&& other) noexcept;

	virtual ~ExchangeRates();

	valute_type copy_valutes() noexcept;	

	reference valutes() noexcept;

	const_reference valutes() const noexcept;

	date_reference date() noexcept;

	date_const_reference date() const noexcept;

	void send_request_to_server();

	void send_request_to_server(const Date& date);	

private:

	WSADATA wsaData;
	SOCKET clientSocket = INVALID_SOCKET;
	sockaddr_in sockAddr;
	struct hostent* host = nullptr;
	Date fieldDate;
	std::unordered_map<std::string, Valute> fieldValutes;
};