#############################################################
## SkeletonSummonHit.fx
#############################################################

FXInfo
LifeSpan	80

############################################################

Condition
	On 	Time
	Time 	0

	Event

		ENAME	Base
		Type	start
		At	root

		Bhvr 	:DarkScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave
		Sound miasmaburst 100 100 .6

		lifeSpan	100
	End

End

Condition
	On 	Time
	Time 	0

	Event

		ENAME	BaseTrail
		Type	start
		At	root

		Part1	:CreateDarknessTrail.part
		Part	:CreateDarknessTrail_Subtractive.part
		Part2	:CreateDarknessTrail2.part
		Part	:CreateDarknessTrail2_Subtractive.part
		Sound miasma_loop 100 100 .5

		lifeSpan	40

	End

	Event

		Type	start
		At	root

		Part	:CircleBones.part
		Part	:CircleBones1.part
		Part	:CircleBones2.part

		lifeSpan	40

	End

	Event
		EName	darkskirt
		Type	Start
		At	ROOT

		Part	:2CreateDarknessPuddle2.part
		Part	:CreateDarknessPuddle2Flat_Subtractive.part
		Part	:CreateDarknessPuddle2Flat.part
		Part	:2CreateDarknessPuddle.part

		lifeSpan	40
	End

End

#############################################################

End