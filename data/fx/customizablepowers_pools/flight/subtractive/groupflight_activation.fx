#############################################################
## GroupFlight_Activation.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
End


#############################################################

Condition
	On	Time
	Time	10

	Event
		ENAME 	WepLCastCloud
		Type 	Local
		At 	Origin
		Sound groupfly3 80 80 .75

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Hips

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		50
			PhysForcePower		40
			PhysForcePowerJitter	10
		End
		Lifespan	25
	End
End

Condition
	On	Time
	Time	15

	Event
		ENAME	WepLCastCloud
		Type	Local
		At	WepL
		part3	:FlightCastCloud01.part
		Pmagnet Chest
		Lifespan 9
	End

End

Condition
	On	Time
	Time	22

	Event
		ENAME	WepLCastRays
		Type	Local
		At	WepL
		part1	:FlightCastGlow01.part
		part2	:FlightCastRays01.part
		Pmagnet Chest
		Lifespan 28
	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	WepRCastCloud
		Type	Local
		At	WepR
		part3	:FlightCastCloud01.part
		Pmagnet Chest
		Lifespan 9
	End

End

Condition
	On	Time
	Time	22

	Event
		ENAME	WepRCastRays
		Type	Local
		At	WepR
		part1	:FlightCastGlow01.part
		part2	:FlightCastRays02.part
		Pmagnet Chest
		Lifespan 28
	End

End

Condition
	On	Time
	Time	23

	Event
		ENAME	CloudBurstR
		Type	Local
		At	WepR
		Bhvr	Behaviors/GroupFlyFadeIn.bhvr
		part2	:FlightCloudSmall01.part
		Pmagnet Chest
		Lifespan 20
	End

End

Condition
	On	Time
	Time	23

	Event
		ENAME	CloudBurstL
		Type	Local
		At	WepL
		Bhvr	Behaviors/GroupFlyFadeIn.bhvr
		part2	:FlightCloudSmall01.part
		Pmagnet Chest
		Lifespan 20
	End

End

Condition
	On	Time
	Time	32

	Event
		ENAME	Ring
		Type	Local
		At	Chest
		Bhvr	Behaviors/GroupFlyFadeIn.bhvr
		part1	:FlightRing.part
		Pmagnet Chest
		Sound groupfly_loop 88 88 .1
	End

	Event
		Ename	LevitationForce
		Type	Posit
		At	Hips

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		60
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
	End

	Event
		Ename	LevitationForce2
		Type	Local
		At	Hips

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		60
			PhysForcePower		20
			PhysForcePowerJitter	5

			PhysForceSpeedScaleMax	100
		End
	End
End

Condition
	On	Time
	Time	39

	Event
		ENAME	Sphere
		Type	Local
		At	Chest
		Bhvr	Behaviors/GroupFlyFadeIn.bhvr
		part1	:FlightCloud01.part
		part2	:FlightStars01.part
		part3	:FlightGlow01.part
		part4	:FlightRays01.part
		Pmagnet Chest
		Sound glow6_loop 88 88 .4
		#Lifespan 29
	End

End


End