#include "test.h"

typedef struct JSONParser
{
private:
	const char *json;
	const char *next;
	const char *jsonEnd;

public:
	JSONParser(const char *toParse);
	void nextChar();
	void skipWhite();
	void match(char x, bool skip);
	char currentChar();
	char *literal();
	char *string();
	size_t number();
} JSONParser;

extern size_t power10(size_t power);
extern JSONAtom *literal(JSONParser *parser);
extern JSONAtom *number(JSONParser *parser);

void testParserViability()
{
	JSONParser *parser = new JSONParser("");
	delete parser;
}

void testPower10()
{
	assertIntEqual(power10(0), 1);
	assertIntEqual(power10(2), 100);
	assertIntEqual(power10(4), 10000);
}

#define TRY(tests) \
try \
{ \
	atom = literal(parser); \
	assertNotNull(atom); \
	tests; \
	delete atom; \
} \
catch (JSONParserError &err) \
{ \
	delete parser; \
	fail(err.error()); \
} \
catch (JSONTypeError &err) \
{ \
	delete atom; \
	delete parser; \
	fail(err.error()); \
}

void testLiteral()
{
	JSONParser *parser;
	JSONAtom *atom;

	parser = new JSONParser("true ");
	TRY(assertTrue(atom->asBool()));
	delete parser;

	parser = new JSONParser("false ");
	TRY(assertFalse(atom->asBool()));
	delete parser;

	parser = new JSONParser("null ");
	TRY(assertNull(atom->asNull()));
	delete parser;

	parser = new JSONParser("invalid ");
	try
	{
		atom = literal(parser);
		delete atom;
		delete parser;
		fail("The parser failed to throw an exception on invalid literal");
	}
	catch (JSONParserError &err)
	{
	}
	delete parser;
}

#undef TRY
#define TRY(tests) \
try \
{ \
	atom = number(parser); \
	assertNotNull(atom); \
	tests; \
	delete atom; \
} \
catch (JSONParserError &err) \
{ \
	delete parser; \
	fail(err.error()); \
} \
catch (JSONTypeError &err) \
{ \
	delete atom; \
	delete parser; \
	fail(err.error()); \
}

#define TRY_SHOULD_FAIL() \
try \
{ \
	atom = number(parser); \
	delete atom; \
	delete parser; \
	fail("The parser failed to throw an exception on invalid number"); \
} \
catch (JSONParserError &err) \
{ \
}

void testIntNumber()
{
	JSONParser *parser;
	JSONAtom *atom;

	parser = new JSONParser("0 ");
	TRY(assertIntEqual(atom->asInt(), 0));
	delete parser;

	parser = new JSONParser("190 ");
	TRY(assertIntEqual(atom->asInt(), 190));
	delete parser;

	parser = new JSONParser("-190 ");
	TRY(assertIntEqual(atom->asInt(), -190));
	delete parser;

	parser = new JSONParser("-0 ");
	TRY(assertIntEqual(atom->asInt(), -0));
	delete parser;

	parser = new JSONParser("19e1 ");
	TRY(assertIntEqual(atom->asInt(), 190));
	delete parser;

	parser = new JSONParser("190e-1 ");
	TRY(assertIntEqual(atom->asInt(), 19));

	parser = new JSONParser("00 ");
	TRY_SHOULD_FAIL();
	delete parser;

	parser = new JSONParser("0e00 ");
	TRY_SHOULD_FAIL();
	delete parser;
}

void testFloatNumber()
{
	JSONParser *parser;
	JSONAtom *atom;

	parser = new JSONParser("0.0 ");
	TRY(assertDoubleEqual(atom->asFloat(), 0.0));
	delete parser;

	parser = new JSONParser("190.0 ");
	TRY(assertDoubleEqual(atom->asFloat(), 190.0));
	delete parser;

	parser = new JSONParser("-0.0 ");
	TRY(assertDoubleEqual(atom->asFloat(), -0.0));
	delete parser;

	parser = new JSONParser("-190.0 ");
	TRY(assertDoubleEqual(atom->asFloat(), -190.0));
	delete parser;

	parser = new JSONParser("19.0e1 ");
	TRY(assertDoubleEqual(atom->asFloat(), 190.0));
	delete parser;

	parser = new JSONParser("19.0e-1 ");
	TRY(assertDoubleEqual(atom->asFloat(), 1.9));
	delete parser;

	parser = new JSONParser("19.0e+0 ");
	TRY(assertDoubleEqual(atom->asFloat(), 19.0));
	delete parser;

	parser = new JSONParser("00.0 ");
	TRY_SHOULD_FAIL();
	delete parser;

	parser = new JSONParser("0.00 ");
	TRY_SHOULD_FAIL();
	delete parser;

	parser = new JSONParser("0.0e00 ");
	TRY_SHOULD_FAIL();
	delete parser;
}

#undef TRY_SHOULD_FAIL
#undef TRY

#ifdef __cplusplus
extern "C"
{
#endif

BEGIN_REGISTER_TESTS()
	TEST(testParserViability)
	TEST(testPower10)
	TEST(testLiteral)
	TEST(testIntNumber)
	TEST(testFloatNumber)
END_REGISTER_TESTS()

#ifdef __cplusplus
}
#endif
