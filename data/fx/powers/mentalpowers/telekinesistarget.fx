#############################################################
## TelekinesisTarget.fx
#############################################################

FxInfo

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	poo
		Type	start
		At	root
		bhvr	behaviors\LevitateScale.bhvr
		part1	Powers\AirControl\LevitateMistBright2.part
		part3	Powers\AirControl\EnsnareRing1.part
		geom	AirFunnel03
		PMAGNET	Head
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	local
		At	poo
		altpiv	1
		part1	Powers\AirControl\LevitateSplash.part
		part2	Powers\AirControl\LevitateSteam.part
		part3	Powers\AirControl\LevitateRocks.part
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	LevitationForce2
		Type	Local
		At	Hips
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		40
			PhysForcePowerJitter	10
		End
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	FootR
		part1	Powers\Flight\FlightCloud.part
		part2	Powers\Flight\FlightCloudSteam.part
		part3	Powers\Flight\FlightSteam.part
		part4	Powers\Flight\FlightCloudSteam2.part
		Part5	Powers\Flight\FlightBodyMist.part
	End

	Event
		Type	Local
		At	FootL
		part1	Powers\Flight\FlightCloud.part
		part2	Powers\Flight\FlightCloudSteam.part
		part3	Powers\Flight\FlightSteam.part
		part4	Powers\Flight\FlightCloudSteam2.part
		Part5	Powers\Flight\FlightBodyMist.part
	End

	Event
		Type	Local
		At	WepL
		part1	Powers\Flight\FlightCloud.part
		part2	Powers\Flight\FlightCloudSteam.part
		part3	Powers\Flight\FlightSteam.part
		part4	Powers\Flight\FlightCloudSteam2.part
		Part5	Powers\Flight\FlightBodyMist.part
	End

	Event
		Type	Local
		At	WepR
		part1	Powers\Flight\FlightCloud.part
		part2	Powers\Flight\FlightCloudSteam.part
		part3	Powers\Flight\FlightSteam.part
		part4	Powers\Flight\FlightCloudSteam2.part
		Part5	Powers\Flight\FlightBodyMist.part
	End
End

#############################################################

End