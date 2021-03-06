#include "turbodbc/field_translators/int64_translator.h"

#include <gtest/gtest.h>


using turbodbc::field_translators::int64_translator;

TEST(Int64TranslatorTest, MakeField)
{
	cpp_odbc::multi_value_buffer buffer(8, 1);
	auto element = buffer[0];
	element.indicator = 8;
	auto const & as_const = buffer;

	int64_translator const translator;

	*reinterpret_cast<long *>(element.data_pointer) = 42;
	EXPECT_EQ(turbodbc::field{42l}, *(translator.make_field(as_const[0])));
}


TEST(Int64TranslatorTest, SetField)
{
	int64_translator const translator;

	cpp_odbc::multi_value_buffer buffer(8, 1);
	auto element = buffer[0];

	translator.set_field(element, {turbodbc::field{42l}});
	EXPECT_EQ(42, *reinterpret_cast<long *>(element.data_pointer));
	EXPECT_EQ(8, element.indicator);
}
