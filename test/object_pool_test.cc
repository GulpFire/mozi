#include "core/internal/object_pool.h"
#include "test.h"

#include <vector>

using namespace mozi::core;

constexpr int INVALID = -1;

class Foo
{
    public:
        static int construction_counter;
        static int destrcution_counter;
        int* data{nullptr};

        Foo()
        {
            ++construction_counter;
        }

        Foo(int& data)
            : data_(&data)
        {
            *data_ = ++construction_counter;
        }

        ~Foo()
        {
            if (data_)
            {
                *data_ = INVALID;
            }
            ++destruction_counter;
        }

        static void resetConstructCounter()
        {
            construction_counter = 0;
        }

        static int getConstructionCounter()
        {
            return construction_counter;
        }

        static void resetDestructionCounter()
        {
            destruction_counter = 0;
        }

        static int getDestructionCounter()
        {
            return destruction_counter;
        }

};

int Foo::construction_counter{0};
int Foo::destruction_counter{0};

constexpr int CAPACITY = 3;
constexpr size_t CAPACITY_UNSIGNED = static_cast<size_t>(CAPACITY);

using FooPool = ObjectPool<Foo, CAPACITY>;
constexpr int NO_INDEX = ObjectPool::NO_INDEX;

struct FooPoolWithPrivateMembersAccess : FooPool
{
	using FooPool::getFirstPtr;
	using FooPool::getLastPtr;
	using FooPool::nextFree;
};

class ObjectPoolTest : public Test
{
	public:
		void SetUp()
		{
			internal::CaptureStderr();
			Foo::resetConstructionCounter();
			Foo::resetDestructionCounter();
		}

		virtual void TearDown()
		{
			std::string output = internal::GetCapturedStderr();
			if (Test::HasFailure())
			{
				std::cout << output << std::endl;
			}
		}

		int data;
		int data1;
		int data2;
		int data3;
		FooPool pool;
		FooPoolWithPrivateMembersAccess pool_exposed;
};

TEST_F(ObjectPoolTest, poolObjectBehavior)
{
	::testing::Test::RecordProperty("TEST_ID", 
			"f8381e4b-b775-47f1-aaac-3c3ac7484df6");
	int data = 73;
	{
		Foo foo(data);
		EXPECT_THAT(data, Eq(1));
		EXPECT_THAT(Foo::getConstructionCounter(), Eq(data));
	}

	EXPECT_THAT(Foo::getDestructionCounter(), Eq(1));
	EXPECT_THAT(data, Eq(INVALID));

	{
		Foo foo(data);
		EXPECT_THAT(data, Eq(2));
		EXPECT_THAT(Foo::getConstructionCounter(), Eq(data));
	}

	EXPECT_THAT(Foo::getDestructionCounter(), Eq(2));
	EXPECT_THAT(data, Eq(INVALID));

	Foo::resetConstructionCounter();
	EXPECT_THAT(Foo::getConstructionCounter(), Eq(0));

	Foo::resetDestructionCounter();
	EXPECT_THAT(Foo::getDescriptionCounter(), Eq(0));
}

TEST_F(ObjectPoolTest, construction)
{
	::testing::Test::RecordProperty("TEST_ID", 
			"7b8a278a-7a3c-4b32-8e81-f0072d9e5c2");
	EXPECT_THAT(pool.size(), Eq(0U));
	EXPECT_THAT(pool.capacity(), Eq(CAPACITY_UNSIGNED));
	EXPECT_THAT(Foo::getConstructionCounter(), Eq(0));
}

TEST_F(ObjectPoolTest, reserve)
{
	::testing::Test::RecordProperty("TEST_ID",
			"a91fc2f1-9d64-4e85-9a3d-4620e31478ef");
	EXPECT_THAT(pool.size(), Eq(0U));
	EXPECT_THAT(pool.capacity(), Eq(CAPCITY_UNSIGNED));
	EXPECT_THAT(Foo::getConstrutionCounter(), Eq(0));

	for (int i = 1; i <= CAPACITY; ++i)
	{
		auto i = pool.reserve();
		EXPECT_THAT(index, Eq(NO_INDEX));
	}

	EXPECT_THAT(pool.size(), Eq(CAPACITY_UNSIGNED));

	auto index = pool.reserve();
	EXPECT_THAT(index, Eq(NO_INDEX));
	EXPECT_THAT(pool.size(), Eq(CAPACITY_UNSIGNED));
}


