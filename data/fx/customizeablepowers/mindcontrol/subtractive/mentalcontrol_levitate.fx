#############################################################
## MentalControl_Levitate.fx
#############################################################

FxInfo
Lifespan 100

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	2

	Event
		ENAME	poo
		Type	start
		At	root
		bhvr	:LevitateScale2.bhvr
		Part	:LevitateMistBright2.part
		Part	:EnsnareRing1.part
		geom	AirFunnel03
		PMAGNET	Head
		Sound levitate4 80 80 0.8
		Lifespan 15
	End

	Event
		ENAME	poo2
		Type	local
		At	poo
		altpiv	1
		Part	:LevitateSplash.part
		Part	:LevitateSteam.part
		Lifespan 9
	End

	Event
		ENAME	poo3
		Type	local
		At	poo
		altpiv	1
		Part	:LevitateRocks.part
		Lifespan 28
	End

	Event
		Ename	LevitationForce2
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 3 0
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		125
			PhysForcePowerJitter	25
		End
		Lifespan 15
	End
End

#############################################################

Condition
	On	Time
	Time	2

	Event
		Ename   targetchill
		Type	Local
		At	FootR
		Part	:FlightCloud.part
		Part	:FlightCloudSteam.part
		Part	:FlightSteam.part
		Part	:FlightCloudSteam2.part
		Part	:FlightBodyMist.part
	End

	Event
		Ename   targetchill
		Type	Local
		At	FootL
		Part	:FlightCloud.part
		Part	:FlightCloudSteam.part
		Part	:FlightSteam.part
		Part	:FlightCloudSteam2.part
		Part	:FlightBodyMist.part
	End

	Event
		Ename   targetchill
		Type	Local
		At	WepL
		Part	:FlightCloud.part
		Part	:FlightCloudSteam.part
		Part	:FlightSteam.part
		Part	:FlightCloudSteam2.part
		Part	:FlightBodyMist.part
	End

	Event
		Ename   targetchill
		Type	Local
		At	WepR
		Part	:FlightCloud.part
		Part	:FlightCloudSteam.part
		Part	:FlightSteam.part
		Part	:FlightCloudSteam2.part
		Part	:FlightBodyMist.part
	End
End

#############################################################

End