#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	Origin
		part1	:FlightCloud.part
		#part2	:FlightCloudSteam.part
		#part3	:FlightSteam.part
		#part4	:FlightCloudSteam2.part
		#Part5	:FlightBodyMist.part
	End
End

End			