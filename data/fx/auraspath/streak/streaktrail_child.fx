#############################################################
## StreakTrail_CHILD.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Anchor
		Type	Local
		At	Root
		BhvrOverride
			Alpha 		1
			Scale		0.85 0.85 0.85
			PositionOffset	0 0.5 0
		End
		Geom	FX_Streak_Anchor
	End

	Event
		Type	Local
		At	Anchor
		AltPiv	2
		Part	:StreakTrail_Alt.part
	End

	Event
		Type	Local
		At	Anchor
		AltPiv	4
		Part	:StreakTrail_Alt.part
	End

	Event
		Type	Local
		At	Anchor
		AltPiv	6
		Part	:StreakTrail_Alt.part
	End
End

#########################################################

End