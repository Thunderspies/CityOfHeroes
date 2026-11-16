#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################
Condition
	On 	Time
	Time	3
	Event
		Type	Local
		At	Root
		Sound 	Brawling_SpinningStrike_Attack_01 200 100 .8
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepR
		Part	:Hand_Trail_Small.part
		Part	:Hand_Trail_Additive_Small.part
		Lifespan 9
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	FootR
		Part	:Hand_Trail.part
		Part	:Hand_Trail_Additive_Small.part
		Lifespan 18
	End

End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 2.0 -10.0
			PYRRotate	90 0 90
		End
		Part	:Air_Dust_Traveling.part
		Part	:Air_Dust_Traveling_Additive.part
		Lifespan 3
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.5 -10.0
			PYRRotate	0 90 180

		End
		Part	:Air_Dust_Traveling_Long.part
		Part	:Air_Dust_Traveling_Additive.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 2.0
		End
		Part	:Ground_Dust_Traveling.part
		Lifespan 7
	End
End

#############################################################

End