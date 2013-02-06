#pragma once

struct UserInfo
{
	CString name;
	CString pass;
};

struct PassengerInfo
{
	CString name;
	CString passTyp;
	CString passNo;
	CString mobileNo;
	CString seatTyp;
};

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
	CString isQueue;
	CString queueNo;
};

struct QueryInfo
{
	CString departDate;
	CString fromStation;
	CString toStation;
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

	CArray<PassengerInfo> passengers;

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
