#############################################################
## MentalControl_Levitate.fx
#############################################################

FxInfo
LifeSpan	100

##############################################################

Input
	InpName	Hips
End

##############################################################


#######################################################
#Levitate
#######################################################

Condition
	On	Time
	Time	2

	Event
		ENAME	poo
		Type	start
		At	root

		geom	AirFunnel03
		bhvr	behaviors/LevitateScale2.bhvr

		part1	Powers/AirControl/LevitateMistBright2.part
			#part2	Powers/AirControl/LevitateMistBright.part
		part3	Powers/AirControl/EnsnareRing1.part
		PMAGNET	Head
		Sound levitate4 80 80 .8
	End


	Event
		ENAME	poo2
		Type	local
		At	poo
		altpiv	1
		part1	Powers/AirControl/LevitateSplash.part
		part2	Powers/AirControl/LevitateSteam.part

	End

	Event
		ENAME	poo3
		Type	local
		At	poo
		altpiv	1

		part3	Powers/AirControl/LevitateRocks.part

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

		Lifespan	15

	End

#######################################################################################
#Flight
#######################################


	Event
		Ename   targetchill
		Type	Local
		At	FootR
		part1	Powers/Flight/FlightCloud.part
		part2	Powers/Flight/FlightCloudSteam.part
		part3	Powers/Flight/FlightSteam.part
		part4	Powers/Flight/FlightCloudSteam2.part
		Part5	Powers/Flight/FlightBodyMist.part
		#Sound	flight2_loop 100.0 30.0 .46

	End

	Event
		Ename   targetchill
		Type	Local
		At	FootL
		part1	Powers/Flight/FlightCloud.part
		part2	Powers/Flight/FlightCloudSteam.part
		part3	Powers/Flight/FlightSteam.part
		part4	Powers/Flight/FlightCloudSteam2.part
		Part5	Powers/Flight/FlightBodyMist.part
		#Sound	flight1x_loop 100.0 30.0 .42
	End

	Event
		Ename   targetchill
		Type	Local
		At	WepL
		part1	Powers/Flight/FlightCloud.part
		part2	Powers/Flight/FlightCloudSteam.part
		part3	Powers/Flight/FlightSteam.part
		part4	Powers/Flight/FlightCloudSteam2.part
		Part5	Powers/Flight/FlightBodyMist.part
	End

	Event
		Ename   targetchill
		Type	Local
		At	WepR
		part1	Powers/Flight/FlightCloud.part
		part2	Powers/Flight/FlightCloudSteam.part
		part3	Powers/Flight/FlightSteam.part
		part4	Powers/Flight/FlightCloudSteam2.part
		Part5	Powers/Flight/FlightBodyMist.part
	End

End
####################################################


Condition
	On 	Time
	Time	17

	Event
		Ename 	Poo
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	30

	Event
		Ename 	Poo3
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	13

	Event
		Ename 	Poo2
		Type	Destroy
	End

End



End