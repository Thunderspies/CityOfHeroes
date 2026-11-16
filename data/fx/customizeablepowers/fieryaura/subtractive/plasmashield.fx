#############################################################
## PlasmaShield.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound flasharrowhit 70 70 .85
		Sound ignite2 50 50 .5

	End

	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 .5
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	Local
		At	head

		part	:PlasmaFireCore.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part	:FireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		part	:FireCore4.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part	:FireCoreTrail2.part
		Part	:FireCoreSparks.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part	:FireCore1.part
		part	:FireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part	:FireCore1.part
		part	:FireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part	:FireCore1Feet.part
		part	:FireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part	:FireCore1Feet.part
		part	:FireCoreTrailFeet.part
	End
End

#############################################################

End