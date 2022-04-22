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

		for (size_t i = 1; i < mode.size(); i++)
		{
			switch (mode[i])
			{
			case 'i':
				_invisible = val;
				break;
			case 'r':
				if (val || _OP)
					_restricted = val;
				break;
			case 'o':
				if (!val)
					_OP = val;
				break;
			default:
				break;
			}
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