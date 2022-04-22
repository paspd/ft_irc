#ifndef CLIENTMODE_HPP
#define CLIENTMODE_HPP

class ClientMode
{
private:
	//i
	bool _invisible;

	//r
	bool _restricted;

	//o
	bool _OP;
public:
	void setMode(std::string const &mode) {
		if (mode.size() < 2)
			return ;
		bool val = (mode[0] == '+' ? true : false);
		
		switch (mode[2])
		{
		case 'i':
			_invisible = val;
			break;
		case 'r':
			_restricted = val;
			break;
		case 'p':
			_OP = val;
			break;
		default:
			break;
		}
	}

	//i r o
	bool getMode(char c) {
		if (c == 'i')
			return _invisible;
		else if (c == 'r')
			return _restricted;
		else if (c == 'o')
			return _OP;
		return false;
	}

	void reset() {
		_invisible = false;
		_restricted = false;
		_OP = false;
	}

	ClientMode(/* args */){};
	~ClientMode(){};
};


#endif