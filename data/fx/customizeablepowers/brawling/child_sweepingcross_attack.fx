#############################################################
## CHILD_SweepingCross_Attack.fx
#############################################################

FxInfo
Lifespan 30

#############################################################
Condition
	On	Time
	Time	12

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
		EName	SwooshPart
		Type	Start
		At	WindAnchor
		Flags	OneAtATime
		Part	:Air_Dust_Traveling_Small.part
		Part	:Air_Dust_Traveling_Additive_Small.part
		Part	:Hand_Dust.part
		LookAt	T_Chest
		Lifespan 8
	End
End

#############################################################

End