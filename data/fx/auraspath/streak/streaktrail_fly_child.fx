#############################################################
## FairyDust_Body.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0 6.3 0
#		End
#		Part	:StreakTrail_HiliteTop.part
#	End
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0 5.6 0
#		End
#		Part	:StreakTrail.part
#	End
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0 4 0
#		End
#		Part	:StreakTrail.part
#	End
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0 2.4 0
#		End
#		Part	:StreakTrail.part
#	End
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0 1.7 0
#		End
#		Part	:StreakTrail_HiliteBottom.part
#	End

	Event
		EName	Anchor
		Type	Local
		At	Root
		BhvrOverride
			Alpha 		1
			Scale		0.85 0.85 0.85
			PositionOffset	0 1.5 0
		End
		Geom	FX_Streak_Anchor
	End

#	Event
#		Type	Local
#		At	Anchor
#		AltPiv	1
#		Part	:StreakTrail_Alt_Hilite.part
#	End

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

#	Event
#		Type	Local
#		At	Anchor
#		AltPiv	7
#		Part	:StreakTrail_Alt_Hilite.part
#	End
End

#########################################################

End