#ifndef CLIENTMODE_HPP
#define CLIENTMODE_HPP

class ClientMode
{
private:
	//a
	bool _away;

	//i
	bool _invisible;

	//w
	bool _WALLOPs;

	//r
	bool _restricted;

	//o
	bool _OP;
public:
	void setMode(char mode, bool val) {
		switch (mode)
		{
		case 'a':
			_away = val;
			break;
		case 'i':
			_invisible = val;
			break;
		case 'w':
			_WALLOPs = val;
			break;
		case 'r':
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

	//a i w r o
	bool getMode(char c) {
		if (c == 'a')
			return _away;
		else if (c == 'i')
			return _invisible;
		else if (c == 'w')
			return _WALLOPs;
		else if (c == 'r')
			return _restricted;
		else if (c == 'o')
			return _OP;
		return false;
	}

	void reset() {
		_away = false;
		_invisible = false;
		_WALLOPs = false;
		_restricted = false;
		_OP = false;
	}

	ClientMode(/* args */){};
	~ClientMode(){};
};


#endif