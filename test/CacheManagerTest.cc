#define CATCH_CONFIG_MAIN
#include "deps/Catch-1.10.0/catch.hpp"

#include <softcache/FifoCache.hh>
#include <softcache/CacheManager.hh>
#include <string>
#include <sstream>

int g_succFetcherKey;
std::string g_succFetcherValue;
bool successfulFetcher(const int &key, std::string &value)
{
	std::stringstream ss;
	ss << key;
	value = ss.str();

	g_succFetcherKey = key;
	g_succFetcherValue = value;
	return true;
}

int g_unsuccFetcherKey;
std::string g_unsuccFetcherValue;
bool unsuccessfulFetcher(const int &key, std::string &value)
{
	g_unsuccFetcherKey = key;
	g_unsuccFetcherValue = "true";
	(void) value;
	return false;
}

int g_disposedKey;
std::string g_disposedValue;
void disposer(const int &key, std::string &value)
{
	g_disposedKey = key;
	g_disposedValue = value;
}

void resetGlobals()
{
	g_succFetcherKey = 0;
	g_succFetcherValue = "";
	g_unsuccFetcherKey = 0;
	g_unsuccFetcherValue = "";
	g_disposedKey = 0;
	g_disposedValue = "";
}

TEST_CASE("CacheManager", "[manager] [cache]")
{
	int key;
	std::string value;
	sc::FifoCache<int,std::string> cache(2);

	SECTION("Successful fetcher, Size = 0")
	{
		sc::CacheManager<int,std::string> cm(&cache, &successfulFetcher, &disposer);

		REQUIRE(cm.capacity() == 2);
		REQUIRE(cm.size() == 0);
		REQUIRE(cm.empty() == true);
		REQUIRE(cm.full() == false);

		SECTION("Size = 1")
		{
			resetGlobals();
			key = 1;
			REQUIRE(cm.retrieve(key, value) == true);
			REQUIRE(value == "1");
			REQUIRE(g_succFetcherKey == 1);
			REQUIRE(g_succFetcherValue == "1");

			REQUIRE(cm.capacity() == 2);
			REQUIRE(cm.size() == 1);
			REQUIRE(cm.empty() == false);
			REQUIRE(cm.full() == false);

			SECTION("Size = 2")
			{
				resetGlobals();
				key = 2;
				REQUIRE(cm.retrieve(key, value) == true);
				REQUIRE(value == "2");
				REQUIRE(g_succFetcherKey == 2);
				REQUIRE(g_succFetcherValue == "2");

				REQUIRE(cm.capacity() == 2);
				REQUIRE(cm.size() == 2);
				REQUIRE(cm.empty() == false);
				REQUIRE(cm.full() == true);

				resetGlobals();
				key = 3;
				REQUIRE(cm.retrieve(key, value) == true);
				REQUIRE(value == "3");
				REQUIRE(g_succFetcherKey == 3);
				REQUIRE(g_succFetcherValue == "3");

				REQUIRE(cm.capacity() == 2);
				REQUIRE(cm.size() == 2);
				REQUIRE(cm.empty() == false);
				REQUIRE(cm.full() == true);

				SECTION("Size = 0")
				{
					resetGlobals();
					cm.flush();
					REQUIRE(g_disposedKey > 0);
					REQUIRE(g_disposedValue.size() > 0);

					REQUIRE(cm.capacity() == 2);
					REQUIRE(cm.size() == 0);
					REQUIRE(cm.empty() == true);
					REQUIRE(cm.full() == false);
				}
			}

			SECTION("Size = 0")
			{
				resetGlobals();
				cm.discard();
				REQUIRE(g_disposedKey == 1);
				REQUIRE(g_disposedValue == "1");

				REQUIRE(cm.capacity() == 2);
				REQUIRE(cm.size() == 0);
				REQUIRE(cm.empty() == true);
				REQUIRE(cm.full() == false);
			}
		}
	}

	SECTION("Unsuccessful fetcher, Size = 0")
	{
		sc::CacheManager<int,std::string> cm(&cache, &unsuccessfulFetcher, &disposer);

		REQUIRE(cm.capacity() == 2);
		REQUIRE(cm.size() == 0);
		REQUIRE(cm.empty() == true);
		REQUIRE(cm.full() == false);

		resetGlobals();
		key = 1;
		REQUIRE(cm.retrieve(key, value) == false);
		REQUIRE(g_unsuccFetcherKey == 1);
		REQUIRE(g_unsuccFetcherValue == "true");

		REQUIRE(cm.capacity() == 2);
		REQUIRE(cm.size() == 0);
		REQUIRE(cm.empty() == true);
		REQUIRE(cm.full() == false);
	}
}
