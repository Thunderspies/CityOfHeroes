#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	0
	Event
		Ename	WindAnchor
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	-3.0 -1.0 -2.0
		End
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Start
		At	WindAnchor
		Part	:Air_Dust_Traveling_Tiny.part
		Part	:Air_Dust_Traveling_Tiny_Additive.part
		LookAt	T_Chest
		Lifespan 8
	End
End

#############################################################

End