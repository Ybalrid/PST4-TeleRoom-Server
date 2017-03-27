#pragma once


#include <PST4Packets.hpp>

namespace PST4
{
	class DynamicObject
	{
	public:
		DynamicObject(const Vect3f& pos, const Vect3f& s, const Quatf& orient, const size_t& o) : 
			position{pos},
			scale{s},
			orientation{orient},
			owner{o}
		{

		}

		Vect3f getPosition() 
		{
			return position;
		}

		Quatf getOrientation()
		{
			return orientation;
		}

		Vect3f getScale()
		{
			return scale;
		}

		void setPosition(Vect3f p)
		{
			position = p;
		}

		void setScale(Vect3f s)
		{
			scale = s;
		}

		void setOrientation(Quatf o)
		{
			orientation = o;
		}

		bool isOwned()
		{
			return owner != 0;
		}

		size_t getOwner()
		{
			return owner;
		}

		void setOwner(size_t o)
		{
			owner = o;
		}

	private:
		Vect3f position, scale;
		Quatf orientation;
		size_t owner;
	};
}