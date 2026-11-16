#############################################################
## Costume Anim Entity - Shield - Default
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
		Bhvr	:Male_Shields_Roman3.bhvr
		Anim	CustomShield_Roman3_Emblem
	End
End

#############################################################

End