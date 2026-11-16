#############################################################
## Costume Anim Entity - Shield - Roman 1
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
		Bhvr	:Male_Shields_Roman1.bhvr
		Anim	CustomShield_Roman1_02
	End
End

#############################################################

End