#############################################################
## Costume Anim Entity - Bow 02
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	LaserStartOffset
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	-0.375 0.0 0.73
		End
	End

	Event
		EName	LaserEndOffset
		Type	Local
		At	LaserStartOffset
		BhvrOverride
			PositionOffset	-5.0 0.0 0.0
		End
	End

	Event
		EName	LaserSightStart
		Bhvr	Behaviors/InheritGroupTint.bhvr
		Type	Local
		At	LaserStartOffset
		Part1	WEAPONS\LaserSight_FlareCore01.part
		Part1	WEAPONS\LaserSight_FlareGlow01.part
	End

	Event
		EName	LaserSightEnd
		Bhvr	Behaviors/InheritGroupTint.bhvr
		Type	Local
		At	LaserEndOffset
		pOther	LaserStartOffset
		Part1	WEAPONS\LaserSight_BeamGlow01.part
	End
End

#############################################################

End