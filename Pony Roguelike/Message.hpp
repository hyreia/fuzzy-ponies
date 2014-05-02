#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <stdarg.h>
#include <memory>

#pragma warning(push)
#pragma warning(disable: 4996) //vsprintf is unsafe and depricated but vsprintf_s is nonstandard
struct Message
{
	std::string message;
	size_t length;
	Color color;

	Message(Color color, const std::string &str): message(str), color(color)
	{
		length = message.size();
	}
	Message(Color color, const char *msg, ...): color(color)
	{
		int final_n, n = strlen(msg) * 2;

		char *message = NULL;
		va_list ap;
		while(true)
		{
			if(message) delete[] message;
			message = new char[n];
			//message.reset(new char[n]);
			strcpy(&message[0], msg);
			va_start(ap, msg);
			final_n = vsnprintf(&message[0], n, msg, ap);
			va_end(ap);
			if(final_n < 0 || final_n >= n)
				n+= abs(final_n - n+1);
			else break;
		}

		length = strlen( message);
		this->message = message;
		//message = std::string(fmt.get());
	}
	~Message(){ }

	size_t size() const{ return length; }
	const char *c_str() const{ return message.c_str(); }

};

#pragma warning(pop)


#endif