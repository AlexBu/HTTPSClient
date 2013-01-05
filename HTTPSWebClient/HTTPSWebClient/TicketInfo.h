#pragma once

class CTicketInfo
{
public:
	CTicketInfo(void);
	virtual ~CTicketInfo(void);
private:
	//station_train_code=G7031
	CString trainCode;
	//	&train_date=2012-11-18
	CString trainDate;
	//	&seattype_num=
	//	&from_station_telecode=NJH
	CString stationFromCode;
	//	&to_station_telecode=SHH
	CString stationToCode;
	//	&include_student=00
	CString student;
	//	&from_station_telecode_name=%E5%8D%97%E4%BA%AC
	CString stationFromTeName;
	//	&to_station_telecode_name=%E4%B8%8A%E6%B5%B7
	CString stationToTeName;
	//	&round_train_date=2012-11-17
	CString trainRoundDate;
	//	&round_start_time_str=00%3A00--24%3A00
	CString trainRoundTimeStr;
	//	&single_round_type=1
	CString roundType;
	//	&train_pass_type=QB
	CString passType;
	//	&train_class_arr=D%23
	CString trainClass;
	//	&start_time_str=00%3A00--24%3A00
	CString timeStartStr;
	//	&lishi=01%3A48
	CString duration;
	//	&train_start_time=05%3A53
	CString trainStartTime;
	//	&trainno4=54000G703120
	CString trainNo;
	//	&arrive_time=07%3A41
	CString timeArrive;
	//	&from_station_name=%E5%8D%97%E4%BA%AC
	CString stationFromName;
	//	&to_station_name=%E4%B8%8A%E6%B5%B7
	CString stationToName;
	//	&ypInfoDetail=O*****06219*****0025O*****3176M*****0110
	CString infoDetail;
	//	&mmStr=16FD07FFF17260ED85F3ACBF68659498DC6E0E388938EAF24A0A0D92
	CString mmStr;
public:
	//station_train_code=G7031
	void trainCodeSet(CString&);
	//	&train_date=2012-11-18
	void trainDateSet(CString&);
	//	&seattype_num=
	//	&from_station_telecode=NJH
	void stationFromCodeSet(CString&);
	//	&to_station_telecode=SHH
	void stationToCodeSet(CString&);
	//	&include_student=00
	void studentSet(CString&);
	//	&from_station_telecode_name=%E5%8D%97%E4%BA%AC
	void stationFromTeNameSet(CString&);
	//	&to_station_telecode_name=%E4%B8%8A%E6%B5%B7
	void stationToTeNameSet(CString&);
	//	&round_train_date=2012-11-17
	void trainRoundDateSet(CString&);
	//	&round_start_time_str=00%3A00--24%3A00
	void trainRoundTimeStrSet(CString&);
	//	&single_round_type=1
	void roundTypeSet(CString&);
	//	&train_pass_type=QB
	void passTypeSet(CString&);
	//	&train_class_arr=D%23
	void trainClassSet(CString&);
	//	&start_time_str=00%3A00--24%3A00
	void timeStartStrSet(CString&);
	//	&lishi=01%3A48
	void durationSet(CString&);
	//	&train_start_time=05%3A53
	void trainStartTimeSet(CString&);
	//	&trainno4=54000G703120
	void trainNoSet(CString&);
	//	&arrive_time=07%3A41
	void timeArriveSet(CString&);
	//	&from_station_name=%E5%8D%97%E4%BA%AC
	void stationFromNameSet(CString&);
	//	&to_station_name=%E4%B8%8A%E6%B5%B7
	void stationToNameSet(CString&);
	//	&ypInfoDetail=O*****06219*****0025O*****3176M*****0110
	void infoDetailSet(CString&);
	//	&mmStr=16FD07FFF17260ED85F3ACBF68659498DC6E0E388938EAF24A0A0D92
	void mmStrSet(CString&);

	void strBuild(CString&);
};
