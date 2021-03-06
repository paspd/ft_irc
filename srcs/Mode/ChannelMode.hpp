#ifndef CHANNELMODE_HPP
#define CHANNELMODE_HPP

#include <string>

class ChannelMode
{
private:
	//mode i
	bool _inviteOnly;
	
	//mode p
	bool _private;

	//mode t
	bool _topicLocked;

	//mode n
	bool _noExternMSG;

	//mode m
	bool _moderate;

	//mode k
	bool _password;
public:
	void reset() {
		_inviteOnly = false;
		_private = false;
		_topicLocked = false;
		_noExternMSG = false;
		_moderate = false;
		_password = false;
	};

	//i p t n m k
	void setMode(std::string const &mode) {
		bool value = (mode[0] == '+' ? true : false);

		switch (mode[1])
		{
		case 'i': _inviteOnly = value; break;
		case 'p': _private = value; break;
		case 't': _topicLocked = value; break;
		case 'n': _noExternMSG = value; break;
		case 'm': _moderate = value; break;
		case 'k': _password = value; break;
		default:
			break;
		}
	};

	//i p t n m b k
	bool getMode(char const &mode) const {
		switch (mode)
		{
		case 'i': return _inviteOnly;
		case 'p': return _private;
		case 't': return _topicLocked;
		case 'n': return _noExternMSG;
		case 'm': return _moderate;
		case 'k': return _password;
		}
		return false;
	};
	
	ChannelMode() {
		reset();
	};

	~ChannelMode() {};
};

#endif