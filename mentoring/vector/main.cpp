#include "vector.hpp"
#include "test_framework.hpp"

void test_constructors_happy()
{
	TestSuite ctors("=== Тестирование конструкторов ===");

	{
		TestSuite _("Конструктор по умолчанию");
		assert(Vector<int>().size() == 0);
	}

	{
		TestSuite _("Конструктор с размером");
		assert(Vector<int>(3).size() == 3);
	}

	const std::initializer_list test_il{ 1, 2, 3, 4, 5 };
	{
		TestSuite _("Конструктор с initializer_list");
		Vector v(test_il);
		assert(is_containers_eq(v, test_il));
	}

	{
		TestSuite _("Конструктор копирования");
		Vector v1(test_il);
		Vector v2(v1);
		assert(is_containers_eq(v2, test_il));
	}

	{
		TestSuite t("move-assign frees own resources first");

		Tracker::alive = 0; Tracker::reset_throws();
		Vector<Tracker> a{ 1, 2, 3 };
		Vector<Tracker> b{ 7, 8 };
		assert(Tracker::alive == 5);

		a = std::move(b);
		assert(Tracker::alive == 2);
		assert(is_containers_eq(a, std::initializer_list{ 7, 8 }));
	}
}

void test_exceptions()
{
	TestSuite exceptions("=== Тестирование безопасности при исключениях ===");

	{
		TestSuite _("copy-assign strong guarantee (no state change on throw)");

		Tracker::alive = 0; Tracker::reset_throws();
		Vector<Tracker> a{ 1, 2, 3 };
		Vector<Tracker> b{ 10, 20, 30, 40 };
		assert(Tracker::alive == 7);

		Tracker::throw_on_copy_n = 1;
		assert(is_throw([&]{ a = b; }));
		assert(Tracker::alive == 7);
		assert(is_containers_eq(a, std::initializer_list{ 1, 2, 3 }));
	}

}

int main()
{
	test_constructors_happy();
	test_exceptions();
}
