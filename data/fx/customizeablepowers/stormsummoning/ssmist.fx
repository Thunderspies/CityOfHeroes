#########################################################
## Storm Summoning - Steamy Mist
#########################################################

FxInfo

## SOUNDS FX #######################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound SS01 100 100 .8
	End
End

#########################################################

Condition

	On 	Time
	Time	50

	Event
		ENAME	Cloud
		Type	Posit
		At	Root
		part	CustomizeablePowers\StormSummoning\SSMistClouds01.part
		Pmagnet Hair
		LifeSpan 70
	End
End

Condition

	On 	Time
	Time	21

	Event
		ENAME	CloudMed
		Type	Posit
		At	Root
		part	CustomizeablePowers\StormSummoning\SSMistClouds02.part
		LifeSpan 62
	End
End

Condition

	On 	Time
	Time	0

	Event
		ENAME	CloudSmall
		Type	Posit
		At	Root
		part	CustomizeablePowers\StormSummoning\SSMistClouds03.part
		LifeSpan 87
	End
End

Condition

	On 	Time
	Time	0 #70

	Event
		ENAME	Ring
		Type	Posit
		At	Root
		part	CustomizeablePowers\StormSummoning\SSMistFlatRing01.part
	End
End

## PHYSICS FORCES #######################################################

Condition

	On 	Time
	Time	0

	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		8
			PhysForcePower		35
			PhysForcePowerJitter	5
		End

		Lifespan	100
	End
End

Condition

	On 	Time
	Time	21

	Event
		Ename	GravitationalForce
		Type	Local
		At	Chest

		BhvrOverride
			PhysForceType		In
			PhysForceRadius		10
			PhysForcePower		25
			PhysForcePowerJitter	5
		End

		Lifespan	60
	End
End

Condition

	On 	Time
	Time	70

	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		10
			PhysForcePowerJitter	5
		End
	End

	Event
		Ename	pushForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		15
			PhysForcePowerJitter	5
		End

		#Lifespan	100
	End
End

## CLEAN UP #######################################################

Condition
	On 	Time
	Time	200

	Event
		EName 	Cloud
		Type	Destroy
	End

	Event
		EName 	CloudSmall
		Type	Destroy
	End

	Event
		EName 	CloudMed
		Type	Destroy
	End
End

#########################################################

End