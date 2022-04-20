#ifndef OCCUPANTCHANNELMODE_HPP
#define OCCUPANTCHANNELMODE_HPP

class OccupantChannelMode
{
private:
	//mode C
	bool _creator;
	
	//mode o
	bool _operator;

	//mode v
	bool _voicePrivilege;

	

public:
	void reset() {
		_creator = false;
		_operator = false;
		_voicePrivilege = false;
	};

	//C o v
	void setMode(std::string const &mode) {
		bool value = (mode[0] == '+' ? true : false);

		switch (mode[1])
		{
		case 'C': _creator = value; break;
		case 'o': _operator = value; break;
		case 'v': _voicePrivilege = value; break;
		default:
			break;
		}
	};

	//i p s t n m b k
	bool getMode(char const &mode) const {
		switch (mode)
		{
		case 'C': return _creator;
		case 'o': return _operator;
		case 'v': return _voicePrivilege;
		}
		return false;
	};

	OccupantChannelMode() {};
	~OccupantChannelMode() {};
};

#endif