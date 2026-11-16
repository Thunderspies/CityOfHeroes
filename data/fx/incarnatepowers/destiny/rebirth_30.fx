#############################################################

## 	REBIRTH FX

#############################################################

FxInfo
Flags	DontSuppress
Lifespan 80
#########################################################

##	HANDS

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	wepL
		Part1	:HandFlare.part
		Part2	:AbilityDots.part
		Part3	:AbilitySparklingDots.part
#		Sound resurrect 70 70 .9
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Part1	:HandFlare.part
		Part2	:AbilityDots.part
		Part3	:AbilitySparklingDots.part
		Lifespan 45
	End
End

##	RADIALS

Condition
	On 	Time
	Time 	30

	Event
		Type	StartPositOnly
		At	Hips
		BhvrOverride
			PyrRotateJitter	1 5 1
		End
		Part	:Rings_Dropping_LocalFacing.part
		Lifespan 50
	End

End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Hips
		Part	:Dust_Down.part
		Lifespan 45
	End

End

##	OUTER EDGE

Condition
	On	Time
	Time	40

	Event
		Type	StartPositOnly
		At	Root
		Part	:SmokeRing_Up_30.part
		Lifespan	40
		BhvrOverride
			PositionOffset 	0.0 -0.5 0.0
		End
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	StartPositOnly
		At	Root
		Part	:SmokeRing_30.part
		Lifespan	40
		BhvrOverride
			PositionOffset 	0.0 0.8 0.0
		End
	End
End



#########################################################

End