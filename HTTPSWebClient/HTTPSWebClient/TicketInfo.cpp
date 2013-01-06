#include "StdAfx.h"
#include "TicketInfo.h"

CTicketInfo::CTicketInfo(void)
{
}

CTicketInfo::~CTicketInfo(void)
{
}

void CTicketInfo::trainCodeSet( CString& str )
{
	trainCode = str;
}

void CTicketInfo::trainDateSet( CString& str )
{
	trainDate = str;
}

void CTicketInfo::stationFromCodeSet( CString& str )
{
	stationFromCode = str;
}

void CTicketInfo::stationToCodeSet( CString& str )
{
	stationToCode = str;
}

void CTicketInfo::studentSet( CString& )
{
	student = L"00";
}

void CTicketInfo::stationFromTeNameSet( CString& str )
{
	stationFromTeName.Empty();

	// transform post string to MBCS type
	char* buff = NULL;
	int buffSize = 0;
	buffSize = WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), NULL, 0, 0, 0);
	if(buffSize > 0)
	{
		buff = new char[buffSize + 1];
		ZeroMemory(buff, (buffSize + 1)*sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), buff, buffSize, 0, 0);
		for(int i = 0; i < buffSize; i++)
		{
			stationFromTeName.AppendFormat(L"%%%02X", (unsigned char)buff[i]);
		}
		delete []buff;
	}
}

void CTicketInfo::stationToTeNameSet( CString& str )
{
	stationToTeName.Empty();

	// transform post string to MBCS type
	char* buff = NULL;
	int buffSize = 0;
	buffSize = WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), NULL, 0, 0, 0);
	if(buffSize > 0)
	{
		buff = new char[buffSize + 1];
		ZeroMemory(buff, (buffSize + 1)*sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), buff, buffSize, 0, 0);
		for(int i = 0; i < buffSize; i++)
		{
			stationToTeName.AppendFormat(L"%%%02X", (unsigned char)buff[i]);
		}
		delete []buff;
	}
}

void CTicketInfo::trainRoundDateSet( CString& )
{
	CTime today = CTime::GetCurrentTime();
	trainRoundDate = today.Format(L"%Y-%m-%d");
}

void CTicketInfo::trainRoundTimeStrSet( CString& )
{
	trainRoundTimeStr = L"00%3A00--24%3A00";
}

void CTicketInfo::roundTypeSet( CString& )
{
	roundType = L"1";
}

void CTicketInfo::passTypeSet( CString& )
{
	passType = L"QB";
}

void CTicketInfo::trainClassSet( CString& )
{
	trainClass = L"D%23";
}

void CTicketInfo::timeStartStrSet( CString& )
{
	timeStartStr = L"00%3A00--24%3A00";
}

void CTicketInfo::durationSet( CString& str )
{
	duration = str;
	duration.Replace(L":", L"%3A");
}

void CTicketInfo::trainStartTimeSet( CString& str )
{
	trainStartTime = str;
	trainStartTime.Replace(L":", L"%3A");
}

void CTicketInfo::trainNoSet( CString& str )
{
	trainNo = str;
}

void CTicketInfo::timeArriveSet( CString& str )
{
	timeArrive = str;
	timeArrive.Replace(L":", L"%3A");
}

void CTicketInfo::stationFromNameSet( CString& str )
{
	stationFromName.Empty();

	// transform post string to MBCS type
	char* buff = NULL;
	int buffSize = 0;
	buffSize = WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), NULL, 0, 0, 0);
	if(buffSize > 0)
	{
		buff = new char[buffSize + 1];
		ZeroMemory(buff, (buffSize + 1)*sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), buff, buffSize, 0, 0);
		for(int i = 0; i < buffSize; i++)
		{
			stationFromName.AppendFormat(L"%%%02X", (unsigned char)buff[i]);
		}
		delete []buff;
	}
}

void CTicketInfo::stationToNameSet( CString& str )
{
	stationToName.Empty();

	// transform post string to MBCS type
	char* buff = NULL;
	int buffSize = 0;
	buffSize = WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), NULL, 0, 0, 0);
	if(buffSize > 0)
	{
		buff = new char[buffSize + 1];
		ZeroMemory(buff, (buffSize + 1)*sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, str.GetString(), str.GetLength(), buff, buffSize, 0, 0);
		for(int i = 0; i < buffSize; i++)
		{
			stationToName.AppendFormat(L"%%%02X", (unsigned char)buff[i]);
		}
		delete []buff;
	}
}

void CTicketInfo::infoDetailSet( CString& str )
{
	infoDetail = str;
}

void CTicketInfo::mmStrSet( CString& str )
{
	mmStr = str;
}

void CTicketInfo::strBuild( CString& str )
{
	str.Format(L"station_train_code=%s"
		L"&train_date=%s"
		L"&seattype_num="
		L"&from_station_telecode=%s"
		L"&to_station_telecode=%s"
		L"&include_student=%s"
		L"&from_station_telecode_name=%s"
		L"&to_station_telecode_name=%s"
		L"&round_train_date=%s"
		L"&round_start_time_str=%s"
		L"&single_round_type=%s"
		L"&train_pass_type=%s"
		L"&train_class_arr=%s"
		L"&start_time_str=%s"
		L"&lishi=%s"
		L"&train_start_time=%s"
		L"&trainno4=%s"
		L"&arrive_time=%s"
		L"&from_station_name=%s"
		L"&to_station_name=%s"
		L"&ypInfoDetail=%s"
		L"&mmStr=%s",
		//station_train_code=G7031
		trainCode,
		//	&train_date=2012-11-18
		trainDate,
		//	&seattype_num=
		//	&from_station_telecode=NJH
		stationFromCode,
		//	&to_station_telecode=SHH
		stationToCode,
		//	&include_student=00
		student,
		//	&from_station_telecode_name=%E5%8D%97%E4%BA%AC
		stationFromTeName,
		//	&to_station_telecode_name=%E4%B8%8A%E6%B5%B7
		stationToTeName,
		//	&round_train_date=2012-11-17
		trainRoundDate,
		//	&round_start_time_str=00%3A00--24%3A00
		trainRoundTimeStr,
		//	&single_round_type=1
		roundType,
		//	&train_pass_type=QB
		passType,
		//	&train_class_arr=D%23
		trainClass,
		//	&start_time_str=00%3A00--24%3A00
		timeStartStr,
		//	&lishi=01%3A48
		duration,
		//	&train_start_time=05%3A53
		trainStartTime,
		//	&trainno4=54000G703120
		trainNo,
		//	&arrive_time=07%3A41
		timeArrive,
		//	&from_station_name=%E5%8D%97%E4%BA%AC
		stationFromName,
		//	&to_station_name=%E4%B8%8A%E6%B5%B7
		stationToName,
		//	&ypInfoDetail=O*****06219*****0025O*****3176M*****0110
		infoDetail,
		//	&mmStr=16FD07FFF17260ED85F3ACBF68659498DC6E0E388938EAF24A0A0D92
		mmStr
		);
}
