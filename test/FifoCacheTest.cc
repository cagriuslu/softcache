#define CATCH_CONFIG_MAIN
#include "deps/Catch-1.10.0/catch.hpp"

#include <softcache/FifoCache.hh>
#include <string>

TEST_CASE("FifoCache")
{
	sc::FifoCache<int,std::string> cache(5);
	std::pair<int,std::string> pair;

	// size = 0
	REQUIRE(cache.capacity() == 5);
	REQUIRE(cache.size() == 0);
	REQUIRE(cache.empty() == true);
	REQUIRE(cache.full() == false);
	REQUIRE(cache.pull(pair) == false);
	REQUIRE(cache.peak(pair) == false);

	// size = 1
	pair.first = 1;
	pair.second = "first";
	REQUIRE(cache.push(pair) == true);
	REQUIRE(cache.capacity() == 5);
	REQUIRE(cache.size() == 1);
	REQUIRE(cache.empty() == false);
	REQUIRE(cache.full() == false);
	pair.second = "";
	REQUIRE(cache.peak(pair) == true);
	REQUIRE(pair.second == "first");

	// size = 0
	pair.first = 0;
	pair.second = "";
	REQUIRE(cache.pull(pair) == true);
	REQUIRE(pair.first == 1);
	REQUIRE(pair.second == "first");
	REQUIRE(cache.capacity() == 5);
	REQUIRE(cache.size() == 0);
	REQUIRE(cache.empty() == true);
	REQUIRE(cache.full() == false);
	REQUIRE(cache.pull(pair) == false);
	REQUIRE(cache.peak(pair) == false);

	// size = 5
	pair.first = 1;
	pair.second = "first";
	REQUIRE(cache.push(pair) == true);
	pair.first = 2;
	pair.second = "second";
	REQUIRE(cache.push(pair) == true);
	pair.first = 3;
	pair.second = "third";
	REQUIRE(cache.push(pair) == true);
	pair.first = 4;
	pair.second = "forth";
	REQUIRE(cache.push(pair) == true);
	pair.first = 5;
	pair.second = "fifth";
	REQUIRE(cache.push(pair) == true);
	REQUIRE(cache.capacity() == 5);
	REQUIRE(cache.size() == 5);
	REQUIRE(cache.empty() == false);
	REQUIRE(cache.full() == true);

	pair.first = 6;
	pair.second = "sixth";
	REQUIRE(cache.push(pair) == false);
	pair.first = 3;
	pair.second = "third";
	REQUIRE(cache.push(pair) == true);
	pair.first = 0;
	pair.second = "";
	REQUIRE(cache.peak(pair) == false);
	pair.first = 4;
	REQUIRE(cache.peak(pair) == true);
	REQUIRE(pair.second == "forth");
	pair.first = 0;
	pair.second = "";
	REQUIRE(cache.pull(pair) == true);
	REQUIRE(pair.first == 1);
	REQUIRE(pair.second == "first");
	REQUIRE(cache.capacity() == 5);
	REQUIRE(cache.size() == 4);
	REQUIRE(cache.empty() == false);
	REQUIRE(cache.full() == false);
}
