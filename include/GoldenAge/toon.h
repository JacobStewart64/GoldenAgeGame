#pragma once
#include <GoldenAge/toongraphics.h>
#include <irrlicht.h>
#include <string>
#include <sstream>
#include <any>

namespace ga {
	struct random_property {
		unsigned char what;
		unsigned char roll;

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
			os << e.props[i];
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
			os << e.props[i];
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i];
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
			os << e.props[i];
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i];
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
			os << e.props[i];
		}
		os << "\n";
		for (int i = 0; i < 6; i++)
		{
			os << e.sock[i];
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
			os << e.props[i];
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i];
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
			os << e.props[i];
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i];
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
			os << e.props[i];
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i];
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
			os << e.props[i];
		}
		os << "\n";
		for (int i = 0; i < 4; i++)
		{
			os << e.sock[i];
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
			os << e.props[i];
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
			os << e.props[i];
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


	struct inventory {
		std::any items[30];

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
		friend std::ostream& operator<<(std::ostream &os, const  inventory  &e);
		friend std::istream& operator>>(std::istream &os, inventory  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  inventory  &e)
	{
		for (int i = 0; i < 30; i++)
		{
			if (e.items[i].type() == typeid(item))
			{
				os << 1 << "\n";
				os << std::any_cast<item>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(headgear))
			{
				os << 2 << "\n";
				os << std::any_cast<headgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(shouldergear))
			{
				os << 3 << "\n";
				os << std::any_cast<shouldergear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(chestgear))
			{
				os << 4 << "\n";
				os << std::any_cast<chestgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(glovegear))
			{
				os << 5 << "\n";
				os << std::any_cast<glovegear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(beltgear))
			{
				os << 6 << "\n";
				os << std::any_cast<beltgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(leggear))
			{
				os << 7 << "\n";
				os << std::any_cast<leggear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(bootgear))
			{
				os << 8 << "\n";
				os << std::any_cast<bootgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(amuletgear))
			{
				os << 9 << "\n";
				os << std::any_cast<amuletgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(ringgear))
			{
				os << 10 << "\n";
				os << std::any_cast<ringgear>(e.items[i]);
			}
			else
			{
				debug("unkown type of item in inventory");
			}
		}
		os << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, inventory  &e)
	{
		for (int i = 0; i < 30; i++)
		{
			int type;
			os >> type;
			if (type == 1)
			{
				item it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 2)
			{
				headgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 3)
			{
				shouldergear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 4)
			{
				chestgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 5)
			{
				glovegear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 6)
			{
				beltgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 7)
			{
				leggear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 8)
			{
				bootgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 9)
			{
				amuletgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 10)
			{
				ringgear it;
				os >> it;
				e.items[i] = it;
			}
			else
			{
				debug("unkown item type");
			}
		}
		return os;
	}


	struct bank {
		std::any items[100];

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
		friend std::ostream& operator<<(std::ostream &os, const  bank  &e);
		friend std::istream& operator>>(std::istream &os, bank  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  bank  &e)
	{
		for (int i = 0; i < 100; i++)
		{
			if (e.items[i].type() == typeid(item))
			{
				os << 1 << "\n";
				os << std::any_cast<item>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(headgear))
			{
				os << 2 << "\n";
				os << std::any_cast<headgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(shouldergear))
			{
				os << 3 << "\n";
				os << std::any_cast<shouldergear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(chestgear))
			{
				os << 4 << "\n";
				os << std::any_cast<chestgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(glovegear))
			{
				os << 5 << "\n";
				os << std::any_cast<glovegear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(beltgear))
			{
				os << 6 << "\n";
				os << std::any_cast<beltgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(leggear))
			{
				os << 7 << "\n";
				os << std::any_cast<leggear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(bootgear))
			{
				os << 8 << "\n";
				os << std::any_cast<bootgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(amuletgear))
			{
				os << 9 << "\n";
				os << std::any_cast<amuletgear>(e.items[i]);
			}
			else if (e.items[i].type() == typeid(ringgear))
			{
				os << 10 << "\n";
				os << std::any_cast<ringgear>(e.items[i]);
			}
			else
			{
				debug("unkown type of item in inventory");
			}
		}
		os << "\n";
		return os;
	}

	std::istream& operator>>(std::istream &os, bank  &e)
	{
		for (int i = 0; i < 100; i++)
		{
			int type;
			os >> type;
			if (type == 1)
			{
				item it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 2)
			{
				headgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 3)
			{
				shouldergear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 4)
			{
				chestgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 5)
			{
				glovegear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 6)
			{
				beltgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 7)
			{
				leggear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 8)
			{
				bootgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 9)
			{
				amuletgear it;
				os >> it;
				e.items[i] = it;
			}
			else if (type == 10)
			{
				ringgear it;
				os >> it;
				e.items[i] = it;
			}
			else
			{
				debug("unkown item type");
			}
		}
		return os;
	}


	struct quests {
		unsigned char flags[16];

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
		friend std::ostream& operator<<(std::ostream &os, const  quests  &e);
		friend std::istream& operator>>(std::istream &os, quests  &e);
	};

	std::ostream& operator<<(std::ostream &os, const  quests  &e)
	{
		for (int i = 0; i < 16; i++)
		{
			os << e.flags[i];
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
	};


	struct world_position {
		unsigned char row;
		unsigned char col;

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
		inventory ti;
		bank tb;
		quests tq;
		unsigned char job;
		derivedstats tds;
		world_position pos;
		irr::core::vector3df vpos;

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
		os << e.ti << "\n";
		os << e.tb << "\n";
		os << e.tq << "\n";
		os << e.job << "\n";
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
		os >> e.ti;
		os >> e.tb;
		os >> e.tq;
		os >> e.job;
		os >> e.pos;
		os >> e.vpos.X;
		os >> e.vpos.Y;
		os >> e.vpos.Z;
		return os;
	}
};