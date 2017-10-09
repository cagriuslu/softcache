#define CATCH_CONFIG_MAIN
#include "deps/Catch-1.10.0/catch.hpp"

#include <softcache/FixedStack.hh>

TEST_CASE("FixedStack", "[container]")
{
	int value;
	sc::FixedStack<int> stack(2);

	REQUIRE(stack.capacity() == 2);
	REQUIRE(stack.size() == 0);
	REQUIRE(stack.empty() == true);
	REQUIRE(stack.full() == false);
	REQUIRE(stack.pull(value) == false);
	REQUIRE(stack.peak(value) == false);

	SECTION("Size = 1")
	{
		value = 1;
		REQUIRE(stack.push(value) == true);

		REQUIRE(stack.capacity() == 2);
		REQUIRE(stack.size() == 1);
		REQUIRE(stack.empty() == false);
		REQUIRE(stack.full() == false);

		value = 0;
		REQUIRE(stack.peak(value) == true);
		REQUIRE(value == 1);

		SECTION("Size = 2")
		{
			value = 2;
			REQUIRE(stack.push(value) == true);

			REQUIRE(stack.capacity() == 2);
			REQUIRE(stack.size() == 2);
			REQUIRE(stack.empty() == false);
			REQUIRE(stack.full() == true);

			value = 0;
			REQUIRE(stack.peak(value) == true);
			REQUIRE(value == 2);

			value = 3;
			REQUIRE(stack.push(value) == false);

			REQUIRE(stack.capacity() == 2);
			REQUIRE(stack.size() == 2);
			REQUIRE(stack.empty() == false);
			REQUIRE(stack.full() == true);

			SECTION("Size = 1")
			{
				value = 0;
				REQUIRE(stack.pull(value) == true);
				REQUIRE(value == 2);

				REQUIRE(stack.capacity() == 2);
				REQUIRE(stack.size() == 1);
				REQUIRE(stack.empty() == false);
				REQUIRE(stack.full() == false);
			}
		}

		SECTION("Size = 0")
		{
			value = 0;
			REQUIRE(stack.pull(value) == true);
			REQUIRE(value == 1);

			REQUIRE(stack.capacity() == 2);
			REQUIRE(stack.size() == 0);
			REQUIRE(stack.empty() == true);
			REQUIRE(stack.full() == false);
			REQUIRE(stack.pull(value) == false);
			REQUIRE(stack.peak(value) == false);
		}
	}
}
