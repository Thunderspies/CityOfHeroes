#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
Lifespan 90
Flags InheritGeoScale

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	StreakAnchor
		Type	Start
		At	Root
	End

	Event
		Type	Local
		At	StreakAnchor
		BhvrOverride
			PositionOffset	0 5.0 30.0
		End
		Part	:Streaks.part
		Part	:Swoosh.part
	End
End


#############################################################

End