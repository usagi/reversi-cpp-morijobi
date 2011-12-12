// base
template<class stones_type>
class player{
public:
	typedef stones_type stones_type;
	typedef stones_type::value_type::element_type::position_type position_type;

	virtual player(stones_type& stones): stones(stones){}
	virtual ~player(){}
	virtual void update(){}
	virtual const position_type& get_next_position() const
	{ return next_position; }

protected:
	stones_type& stones;
	position_type next_position;
};

// delivered
template<class stones_type>
class computer_player
	: public player<stones_type>
{
public:
	computer_player(stones_type stones)
	{ player::player(stones); }

	void update(){

	}
};
