#define CATCH_CONFIG_MAIN
#include "deps/Catch-1.10.0/catch.hpp"

#include <softcache/LruCache.hh>
#include <string>

TEST_CASE("LruCache", "[lru] [cache] [container]")
{
	sc::LruCache<int,std::string> cache(2);
	std::pair<int,std::string> pair;

	REQUIRE(cache.capacity() == 2);
	REQUIRE(cache.size() == 0);
	REQUIRE(cache.empty() == true);
	REQUIRE(cache.full() == false);
	REQUIRE(cache.pull(pair) == false);
	REQUIRE(cache.peak(pair) == false);

	SECTION("Size = 1")
	{
		pair.first = 1;
		pair.second = "first";
		REQUIRE(cache.push(pair) == true);
		pair.second = "";
		REQUIRE(cache.peak(pair) == true);
		REQUIRE(pair.second == "first");
		REQUIRE(cache.push(pair) == true);

		REQUIRE(cache.capacity() == 2);
		REQUIRE(cache.size() == 1);
		REQUIRE(cache.empty() == false);
		REQUIRE(cache.full() == false);

		pair.first = 0;
		pair.second = "";
		REQUIRE(cache.peak(pair) == false);

		SECTION("Size = 0")
		{
			pair.first = 0;
			pair.second = "";
			REQUIRE(cache.pull(pair) == true);
			REQUIRE(pair.first == 1);
			REQUIRE(pair.second == "first");

			REQUIRE(cache.capacity() == 2);
			REQUIRE(cache.size() == 0);
			REQUIRE(cache.empty() == true);
			REQUIRE(cache.full() == false);
			REQUIRE(cache.pull(pair) == false);
			REQUIRE(cache.peak(pair) == false);
		}

		SECTION("Size = 2")
		{
			pair.first = 2;
			pair.second = "second";
			REQUIRE(cache.push(pair) == true);
			pair.second = "";
			REQUIRE(cache.peak(pair) == true);
			REQUIRE(pair.second == "second");
			REQUIRE(cache.push(pair) == true);

			REQUIRE(cache.capacity() == 2);
			REQUIRE(cache.size() == 2);
			REQUIRE(cache.empty() == false);
			REQUIRE(cache.full() == true);

			pair.first = 3;
			pair.second = "third";
			REQUIRE(cache.push(pair) == false);

			SECTION("Size = 1")
			{
				pair.first = 0;
				pair.second = "";
				REQUIRE(cache.pull(pair) == true);
				REQUIRE(pair.first == 1);
				REQUIRE(pair.second == "first");

				REQUIRE(cache.capacity() == 2);
				REQUIRE(cache.size() == 1);
				REQUIRE(cache.empty() == false);
				REQUIRE(cache.full() == false);
			}
		}
	}
}
