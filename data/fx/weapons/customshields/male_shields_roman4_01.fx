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
		Bhvr	:Male_Shields_Roman4.bhvr
		Anim	CustomShield_Roman4_01
	End
End

#############################################################

End