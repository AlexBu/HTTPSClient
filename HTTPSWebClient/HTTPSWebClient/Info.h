#pragma once

struct LoginInfo
{
	CString rand;
	CString username;
	CString password;
	CString validate;
};

struct OrderInfo
{
	CString errMsg;
	CString orderInfo;
	CString waitTime;
	CString orderId;
	CString ticketStr;
	CString checkHuimd;
	CString check608;
	CString msg;
};

struct QueryInfo
{
	CString departDate;
	CString fromStation;
	CString toStation;
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

struct PassengerInfo
{
	CString passengerTickets;
	CString oldPassengers;
	CString seat;
	CString seat_detail;
	CString ticket;
	CString name;
	CString cardtype;
	CString cardno;
	CString mobileno;
};

struct TicketInfo
{
	CString token;
	CString leftTicketStr;
	CString textfield;
	CString train_date;
	CString train_no;
	CString station_train_code;
	CString from_station_telecode;
	CString to_station_telecode;
	CString bed_level_order_num;
	CString start_time;
	CString end_time;
	CString from_station_name;
	CString to_station_name;
	CString cancel_flag;
	CString id_mode;

	PassengerInfo passengers[5];

	CString checkbox9;
	CString randCode;
	CString reserve_flag;
	CString tFlag;
};

struct TrainInfo
{
	CString trainCode;
	CString trainDate;
	CString stationFromCode;
	CString stationToCode;
	CString student;
	CString stationFromTeName;
	CString stationToTeName;
	CString trainRoundDate;
	CString trainRoundTimeStr;
	CString roundType;
	CString passType;
	CString trainClass;
	CString timeStartStr;
	CString duration;
	CString trainStartTime;
	CString trainNo;
	CString timeArrive;
	CString stationFromName;
	CString stationToName;
	CString stationFromNo;
	CString stationToNo;
	CString infoDetail;
	CString mmStr;
	CString locationCode;
};
