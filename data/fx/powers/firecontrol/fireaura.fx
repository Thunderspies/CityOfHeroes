#########################################################
##	FireAura
FxInfo

##################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At 	Origin
		Sound ignite2 80 80 .9

	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At 	Origin
		Sound firezap2 80 80 .84
	End

	Event
		Type	Local
		At 	Origin
		Bhvr	behaviors/soundFade3.bhvr
		Sound firesword_loop 80 80 .7
		Flags	PowerLoopingSound
		Lifespan 120
	End
End

Condition
	On	Time
	Time	0

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


End