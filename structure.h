#pragma once

namespace pkodev
{
	// Server time structure
	struct time
	{
		// Current client tick
		unsigned __int64 tick;

		// Server time fields
		unsigned int month;
		unsigned int day;
		unsigned int hour;
		unsigned int minute;
		unsigned int second;
		unsigned int millisecond;

		// Constructor
		time() :
			tick(0),
			month(0),
			day(0),
			hour(0),
			minute(0),
			second(0),
			millisecond(0)
		{

		}

	};
}