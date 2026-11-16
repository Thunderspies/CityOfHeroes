#############################################################
## FX_Bat_01.fx
#############################################################

FxInfo
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 0 0 0
			Scale	1 1 1
			#PyrRotate 0 180 0
		End
		Anim	FX_SharkFin
	End
End


#############################################################
End