#############################################################
## Costume Anim Entity - Shield - Roman 4
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
#		Bhvr	:Male_Shields_Knight_01.bhvr
		Anim	CustomShield_PostApoc_Emblem_01
		BhvrOverride
			PositionOffset	-.45 .30 0
			PYRrotate 	0 0 0
			Scale		1.05 1.05 1.05
			TintGeom	1
	End
End

#############################################################

End