#############################################################
## Costume Anim Entity - Shield - Roman 1 Emblems
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
		Bhvr	:Huge_Shields_Roman1.bhvr
		Anim	CustomShield_Roman1_Emblem
	End
End

#############################################################

End