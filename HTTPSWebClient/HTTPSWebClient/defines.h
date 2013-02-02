#pragma once

#define ERROR_OK				(0)
#define ERROR_GENERAL			(-1)
#define ERROR_HTTP				(-2)
#define ERROR_LOGIC				(-3)
#define ERROR_VALIDATE			(-4)
#define ERROR_USERNAME			(-5)
#define ERROR_PASSWORD			(-6)
#define ERROR_OVERLOAD			(-7)
#define ERROR_NOSEAT			(-8)
#define ERROR_ORDER				(-9)
#define ERROR_CANCEL_TOO_MANY	(-10)
#define ERROR_REAL_NAME			(-11)
#define ERROR_USER_CANCEL		(-12)
#define ERROR_IN_QUEUE			(-13)
#define ERROR_SERVER			(-14)
#define ERROR_NOTRAIN			(-15)

enum STATUS
{
	START = 0,
	LOGINV,
	RAND,
	LOGIN,
	QUERY,
	BOOK,
	BOOKV,
	CHECK,
	QUEUE,
	CONFRM,
	WAIT,
	EXIT
};

//O, M, 9, 3, 4, 2, 1
enum SeatType
{
	BUSI_SEAT = '9',
	SUPER_SEAT = 'P',
	FIRST_SEAT = 'M',
	SECOND_SEAT = 'O',
	SUPER_SOFT_SLEEPER = '6',
	SOFT_SLEEPER= '4',
	HARD_SLEEPER = '3',
	SOFT_SEAT = '2',
	HARD_SEAT = '1',
	SUPER_SOFT_SEAT = 'E',
	NO_SEAT = 'X',	// not sure
	OTHER = 'Z'		// not sure
};