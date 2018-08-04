#pragma once
#include <string>
#include <sstream>
#include <any>

namespace ga {
	struct random_property {
		unsigned char what;
		unsigned char roll;

		random_property() {}

		random_property(bool b) : what(0), roll(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  random_property  &e);
		friend std::istream& operator>>(std::istream &os, random_property  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  random_property  &e)
	{
		os << e.what << "\n";
		os << e.roll << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, random_property  &e)
	{
		os >> e.what;
		os >> e.roll;
		return os;
	}


	struct insertable {
		unsigned char id;
		random_property props[3];

		insertable() {}

		insertable(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true) } {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  insertable  &e);
		friend std::istream& operator>>(std::istream &os, insertable  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  insertable  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 3; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, insertable  &e)
	{
		os >> e.id;
		for (int i = 0; i < 3; i++)
		{
			os >> e.props[i];
		}
		return os;
	}


	struct socket {
		bool isactive;
		insertable in;

		socket() {}

		socket(bool b) : isactive(false), in(true) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  socket  &e);
		friend std::istream& operator>>(std::istream &os, socket  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  socket  &e)
	{
		os << e.isactive << "\n";
		os << e.in << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, socket  &e)
	{
		os >> e.isactive;
		os >> e.in;
		return os;
	}


	struct headgear {
		unsigned char id;
		random_property props[4];
		socket sock[4];
		unsigned char grind;
		unsigned char enchant;

		headgear() {}

		headgear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true) },
			sock{ socket(true), socket(true), socket(true), socket(true) }, grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  headgear  &e);
		friend std::istream& operator>>(std::istream &os, headgear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  headgear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i] << "\n";
		}
		os << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, headgear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 4; i++)
		{
			os >> e.props[i];
		}
		for (int i = 0; i < 4; i++)
		{
			os >> e.sock[i];
		}
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct shouldergear {
		unsigned char id;
		random_property props[4];
		socket sock[4];
		unsigned char grind;
		unsigned char enchant;

		shouldergear() {}

		shouldergear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true) },
			sock{ socket(true), socket(true), socket(true), socket(true) }, grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  shouldergear  &e);
		friend std::istream& operator>>(std::istream &os, shouldergear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  shouldergear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i] << "\n";
		}
		os << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, shouldergear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 4; i++)
		{
			os >> e.props[i];
		}
		for (int i = 0; i < 4; i++)
		{
			os >> e.sock[i];
		}
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct chestgear {
		unsigned char id;
		random_property props[7];
		socket sock[6];
		unsigned char grind;
		unsigned char enchant;

		chestgear() {}

		chestgear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true), random_property(true), random_property(true), random_property(true) },
			sock{ socket(true), socket(true), socket(true), socket(true), socket(true), socket(true) }, grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  chestgear  &e);
		friend std::istream& operator>>(std::istream &os, chestgear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  chestgear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 7; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		for (int i = 0; i < 6; i++)
		{
			os << e.sock[i] << "\n";
		}
		os << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, chestgear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 7; i++)
		{
			os >> e.props[i];
		}
		for (int i = 0; i < 6; i++)
		{
			os >> e.sock[i];
		}
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct glovegear {
		unsigned char id;
		random_property props[4];
		socket sock[4];
		unsigned char grind;
		unsigned char enchant;

		glovegear() {}

		glovegear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true) },
			sock{ socket(true), socket(true), socket(true), socket(true) }, grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  glovegear  &e);
		friend std::istream& operator>>(std::istream &os, glovegear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  glovegear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i] << "\n";
		}
		os << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, glovegear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 4; i++)
		{
			os >> e.props[i];
		}
		for (int i = 0; i < 4; i++)
		{
			os >> e.sock[i];
		}
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct beltgear {
		unsigned char id;
		random_property props[4];
		socket sock[4];
		unsigned char grind;
		unsigned char enchant;

		beltgear() {}

		beltgear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true) },
			sock{ socket(true), socket(true), socket(true), socket(true) }, grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  beltgear  &e);
		friend std::istream& operator>>(std::istream &os, beltgear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  beltgear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i] << "\n";
		}
		os << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, beltgear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 4; i++)
		{
			os >> e.props[i];
		}
		for (int i = 0; i < 4; i++)
		{
			os >> e.sock[i];
		}
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct leggear {
		unsigned char id;
		random_property props[4];
		socket sock[4];
		unsigned char grind;
		unsigned char enchant;

		leggear() {}

		leggear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true) },
			sock{ socket(true), socket(true), socket(true), socket(true) }, grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  leggear  &e);
		friend std::istream& operator>>(std::istream &os, leggear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  leggear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i] << "\n";
		}
		os << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, leggear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 4; i++)
		{
			os >> e.props[i];
		}
		for (int i = 0; i < 4; i++)
		{
			os >> e.sock[i];
		}
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct bootgear {
		unsigned char id;
		random_property props[4];
		socket sock[4];
		unsigned char grind;
		unsigned char enchant;
		
		bootgear() {}

		bootgear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true) },
			sock{ socket(true), socket(true), socket(true), socket(true) }, grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  bootgear  &e);
		friend std::istream& operator>>(std::istream &os, bootgear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  bootgear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i] << "\n";
		}
		os << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, bootgear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 4; i++)
		{
			os >> e.props[i];
		}
		for (int i = 0; i < 4; i++)
		{
			os >> e.sock[i];
		}
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct amuletgear {
		unsigned char id;
		random_property props[7];
		socket sock;
		unsigned char grind;
		unsigned char enchant;

		amuletgear() {}

		amuletgear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true), random_property(true), random_property(true), random_property(true) },
			sock(true), grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  amuletgear  &e);
		friend std::istream& operator>>(std::istream &os, amuletgear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  amuletgear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 7; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		os << e.sock << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, amuletgear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 7; i++)
		{
			os >> e.props[i];
		}
		os >> e.sock;
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct ringgear {
		unsigned char id;
		random_property props[4];
		socket sock;
		unsigned char grind;
		unsigned char enchant;

		ringgear() {}

		ringgear(bool b) : id(0), props{ random_property(true), random_property(true), random_property(true), random_property(true) },
			sock(true), grind(0), enchant(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  ringgear  &e);
		friend std::istream& operator>>(std::istream &os, ringgear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  ringgear  &e)
	{
		os << e.id << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.props[i] << "\n";
		}
		os << "\n";
		os << e.sock << "\n";
		os << e.grind << "\n";
		os << e.enchant << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, ringgear  &e)
	{
		os >> e.id;
		for (int i = 0; i < 4; i++)
		{
			os >> e.props[i];
		}
		os >> e.sock;
		os >> e.grind;
		os >> e.enchant;
		return os;
	}


	struct gear {
		headgear head;
		shouldergear shoulder;
		chestgear chest;
		glovegear glove;
		beltgear belt;
		leggear leg;
		bootgear boot;
		amuletgear amulet;
		ringgear ring1;
		ringgear ring2;

		gear() {}

		gear(bool b) : head(true), shoulder(true), chest(true), glove(true), belt(true), leg(true), boot(true), amulet(true), ring1(true), ring2(true) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  gear  &e);
		friend std::istream& operator>>(std::istream &os, gear  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  gear  &e)
	{
		os << e.head << "\n";
		os << e.shoulder << "\n";
		os << e.chest << "\n";
		os << e.glove << "\n";
		os << e.belt << "\n";
		os << e.leg << "\n";
		os << e.boot << "\n";
		os << e.amulet << "\n";
		os << e.ring1 << "\n";
		os << e.ring2 << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, gear  &e)
	{
		os >> e.head;
		os >> e.shoulder;
		os >> e.chest;
		os >> e.glove;
		os >> e.belt;
		os >> e.leg;
		os >> e.boot;
		os >> e.amulet;
		os >> e.ring1;
		os >> e.ring2;
		return os;
	}


	struct skills {
		unsigned char str;
		unsigned char agi;
		unsigned char inte;
		unsigned char bs;
		unsigned char cloth;
		unsigned char leather;
		unsigned char ww;
		unsigned char wc;
		unsigned char mine;
		unsigned char skin;
		unsigned char ench;
		unsigned char merch;

		skills() {}

		skills(bool b) : str(1), agi(1), inte(1), bs(1), cloth(1), leather(1), ww(1), wc(1), mine(1), skin(1), ench(1), merch(1) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  skills  &e);
		friend std::istream& operator>>(std::istream &os, skills  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  skills  &e)
	{
		os << e.str << "\n";
		os << e.agi << "\n";
		os << e.inte << "\n";
		os << e.bs << "\n";
		os << e.cloth << "\n";
		os << e.leather << "\n";
		os << e.ww << "\n";
		os << e.wc << "\n";
		os << e.mine << "\n";
		os << e.skin << "\n";
		os << e.ench << "\n";
		os << e.merch << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, skills  &e)
	{
		os >> e.str;
		os >> e.agi;
		os >> e.inte;
		os >> e.bs;
		os >> e.cloth;
		os >> e.leather;
		os >> e.ww;
		os >> e.wc;
		os >> e.mine;
		os >> e.skin;
		os >> e.ench;
		os >> e.merch;
		return os;
	}


	struct item {
		unsigned char type;
		unsigned char id;

		item() {}

		item(bool b) : type(0), id(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  item  &e);
		friend std::istream& operator>>(std::istream &os, item  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  item  &e)
	{
		os << e.type << "\n";
		os << e.id << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, item  &e)
	{
		os >> e.type;
		os >> e.id;
		return os;
	}


	struct quests {
		unsigned char flags[16];

		quests() {}

		quests(bool b) : flags{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } {}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  quests  &e);
		friend std::istream& operator>>(std::istream &os, quests  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  quests  &e)
	{
		for (int i = 0; i < 16; i++)
		{
			os << e.flags[i] << "\n";
		}
		os << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, quests  &e)
	{
		for (int i = 0; i < 16; i++)
		{
			os >> e.flags[i];
		}
		return os;
	}


	struct derivedstats {
		unsigned int hp;
		unsigned int mp;
		unsigned int ad;
		unsigned int atsp;
		unsigned int ap;
		unsigned int ar;
		unsigned int mr;
		unsigned char ls;

		derivedstats() {}

		derivedstats(bool b) : hp(0), mp(0), ad(0), atsp(0), ap(0), ar(0), mr(0), ls(0) {}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  derivedstats  &e);
		friend std::istream& operator>>(std::istream &os, derivedstats  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  derivedstats  &e)
	{
		os << e.hp << "\n";
		os << e.mp << "\n";
		os << e.ad << "\n";
		os << e.atsp << "\n";
		os << e.ap << "\n";
		os << e.ar << "\n";
		os << e.mr << "\n";
		os << e.ls << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, derivedstats  &e)
	{
		os >> e.hp;
		os >> e.mp;
		os >> e.ad;
		os >> e.atsp;
		os >> e.ap;
		os >> e.ar;
		os >> e.mr;
		os >> e.ls;
		return os;
	}


	struct world_position {
		unsigned char row;
		unsigned char col;

		world_position() {}

		world_position(bool b) : row(0), col(0) {}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  world_position  &e);
		friend std::istream& operator>>(std::istream &os, world_position  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  world_position  &e)
	{
		os << e.row << "\n";
		os << e.col << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, world_position  &e)
	{
		os >> e.row;
		os >> e.col;
		return os;
	}


	struct toon {
		std::string name;
		gear tg;
		skills ts;
		quests tq;
		unsigned char job;
		derivedstats tds;
		world_position pos;
		irr::core::vector3df vpos;

		toon() {}

		toon(bool b) : name(""), tg(true), ts(true), tq(true), job(0), tds(true), pos(true), vpos(250, 500, 250) {}

		toon(std::string& name)
		{
			this->name = name;
		}

		void from_string(std::string str)
		{
			std::stringstream ss(str);
			ss >> *this;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		friend std::ostream& operator<<(std::ostream &os, const  toon  &e);
		friend std::istream& operator>>(std::istream &os, toon  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  toon  &e)
	{
		os << e.tg << "\n";
		os << e.ts << "\n";
		os << e.tq << "\n";
		os << e.job << "\n";
		os << e.tds << "\n";
		os << e.pos << "\n";
		os << e.vpos.X << "\n";
		os << e.vpos.Y << "\n";
		os << e.vpos.Z << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, toon  &e)
	{
		os >> e.tg;
		os >> e.ts;
		os >> e.tq;
		os >> e.job;
		os >> e.tds;
		os >> e.pos;
		os >> e.vpos.X;
		os >> e.vpos.Y;
		os >> e.vpos.Z;
		return os;
	}
};