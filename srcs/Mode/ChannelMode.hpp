#ifndef CHANNELMODE_HPP
#define CHANNELMODE_HPP

class ChannelMode
{
private:
	//mode i
	bool _inviteOnly;
	
	bool _voicePrivilege;

public:
	void reset() {
		_creator = false;
		_operator = false;
		_voicePrivilege = false;
	};

	void setCreator(bool const &rhs) {
		_creator = rhs;
	};
	
	void setOperator(bool const &rhs){
		_operator = rhs;
	};
	
	void setVoicePrivilege(bool const &rhs) {
		_voicePrivilege = rhs;
	};

	bool getCreator() const { return _creator; };
	
	bool getOperator() const { return _operator; };
	
	bool getVoicePrivilege() const { return _voicePrivilege; };

	ChannelMode() {};
	~ChannelMode() {};
};

#endif