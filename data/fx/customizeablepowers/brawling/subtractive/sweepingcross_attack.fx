#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################
Condition
	On 	Time
	Time	1
	Event
		Type	Local
		At	Root
		Sound 	Brawling_SweepingCross_Attack_01 200 100 .8
	End
End

Condition
	On	Time
	Time	11
	Event
		Type	Local
		At	WepR
		Part	:Hand_Buildup_Star.part
		Part	:Hand_Buildup_Ring.part

	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	WepR
		Part	:Hand_Trail.part
		Part	:Hand_Trail_Additive.part
		Lifespan 20
	End

	Event
		Ename	WindAnchor
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	5.0 3.0 0.0
		End
	End
End

Condition
	On	Time
	Time	22

	Event
		Type	Start
		At	WindAnchor
		Part	:Air_Dust_Traveling_Small.part
		Part	:Air_Dust_Traveling_Additive_Small.part
		Part	:Hand_Dust.part
		LookAt	T_Chest
		Lifespan 8
	End
End

#############################################################

End