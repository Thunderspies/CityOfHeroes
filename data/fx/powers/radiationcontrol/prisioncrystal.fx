#########################################################
##	chill touch hit
FxInfo


LifeSpan	120

############################################################

############################################################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	HookUp
		Type	Local
		At	Root
		Geom	RootToHeadAdjustment
		
		LifeSpan	30
	End

	Event
		EName	RingsDown
		Type	Local
		At	HookUp
		Altpiv	1

		Part1	:RadiationPoisoningRingsDown.part

		LifeSpan	20
	End

	Event
		EName	RingsOut
		Type	Local
		At	Root

		Part1	:RadiationPoisoningRingsOut2.part

		LifeSpan	20
	End

End

####################################################################################

Condition
	On 	Time
	Time 	35

	Event
		EName	RingsDown
		Type	Local
		At	root

		Part1	:RadiationPoisoningRingsUp.part

		LifeSpan	22
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName	RingsOut
		Type	Local
		At	Root

		Part1	:RadiationPoisoningRingsIn.part

		LifeSpan	30
	End

End


Condition
	On 	Time
	Time 	25

	Event
		EName	RingsDown
		Type	Local
		At	root

		ChildFx	V_COV/Powers/Heal/HealNoStreak.fx

		LifeSpan	42
	End

End

End			