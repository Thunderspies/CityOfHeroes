#############################################################
## TelekinesisTarget.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	poo
		Type	start
		At	root
		bhvr	behaviors\LevitateScale.bhvr
		BhvrOverride
			Alpha 		50
			TintGeom	1
		End
		part1	:LevitateMistBright2.part
		part3	:EnsnareRing1.part

		geom	Tintable_AirFunnel03
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
		part1	:LevitateSplash.part
		part2	:LevitateSteam.part
		part3	:LevitateRocks.part
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
		part1	:FlightCloud.part
		part2	:FlightCloudSteam.part
		part3	:FlightSteam.part
		part4	:FlightCloudSteam2.part
		Part5	:FlightBodyMist.part
	End

	Event
		Type	Local
		At	FootL
		part1	:FlightCloud.part
		part2	:FlightCloudSteam.part
		part3	:FlightSteam.part
		part4	:FlightCloudSteam2.part
		Part5	:FlightBodyMist.part
	End

	Event
		Type	Local
		At	WepL
		part1	:FlightCloud.part
		part2	:FlightCloudSteam.part
		part3	:FlightSteam.part
		part4	:FlightCloudSteam2.part
		Part5	:FlightBodyMist.part
	End

	Event
		Type	Local
		At	WepR
		part1	:FlightCloud.part
		part2	:FlightCloudSteam.part
		part3	:FlightSteam.part
		part4	:FlightCloudSteam2.part
		Part5	:FlightBodyMist.part
	End
End

#############################################################

End