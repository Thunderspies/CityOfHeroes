#########################################################
##	FireAura
FxInfo

Lifespan	300
###############################################################################

Condition
	On 	Time
	Time	0

	Event
		Type 	Local
		At	Origin
		Sound torch2_loop 100.0 100.0 .4
	End

	Event
		Type 	Local
		At	Origin
		Sound rumble1 100.0 100.0 .75
	End

	Event
		Type 	Local
		At	Origin
		Sound ressurect 100.0 100.0 .7
	End
End

Condition
	On 	Time
	Time	60

	Event
		Type 	Local
		At	Origin
		Sound fireball1 100 100 .85
	End

	Event
		Type 	Local
		At	Origin
		Sound Firesword_loop 100.0 100.0 .44
	End
End

Condition
	On 	Time
	Time	95

	Event
		Type 	Local
		At	Origin
		Sound cinders3 100.0 100.0 .9
	End
End

Condition
	On 	Time
	Time	120

	Event
		Type 	Local
		At	Origin
		Sound aura3 100.0 100.0 .85
	End
End

Condition
	On 	time
	Time	0

	Event
		EName	wing
		Type	Local
		At	Root
		part1	:PhoenixDeath.part
		Sound Ignite2 100.0 100.0 .7
		LifeSpan	85
	End

End

Condition
	On 	time
	Time	132

	Event
		EName	wing
		Type	Local
		At	winghook
		AltPiv	1
		Part	:lightRays.part
		part1	:PhoenixDeath1.part


		LifeSpan	78
	End

	Event
		EName	headSparks
		Type	Local
		At	head
		Part4	:PhoenixSparks.part
		lifeSpan	65

	End

End

Condition
	On 	Time
	Time 	80

	Event
		EName	winghook
		Type	Local
		At	root
		Geom	RoottoHeadAdjustment
		Bhvr	CustomizeablePowers\FieryAura\PhoenixMoveUp.Bhvr
	End

	Event
		EName	Pivot
		Type	Local
		At	head

		part2	:FireCore.part

	End

	Event
		EName	Hips2
		Type	Local
		At	Hips

		part2	:FireCore2.part
		part3	:FireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest

		part2	:FireCore3.part
		part3	:FireCoreTrail2.part
		Part4	:FireCoreSparks2.part
	End

	Event
		EName	Chest
		Type	Local
		At	Head
		part1	:FireCore4.part
		part2	:FireCoreTrail3.part
	End


	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	:FireCore1.part
		part2	:FireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	:FireCore1.part
		part2	:FireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part1	:FireCore1Feet.part
		part2	:FireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part1	:FireCore1Feet.part
		part2	:FireCoreTrailFeet.part
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part

		LifeSpan	75

	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part

		LifeSpan	75
	End

End

Condition
	On	Time
	Time	95

	Event
		EName	Hips
		Type	Local
		At	Hips
		part1	:FireAura2.part
	End

End

Condition
	On	Time
	Time	120

	Event
		EName	Hips
		Type	Local
		At	Hips
		part1	:FireAura2.part
	End

End