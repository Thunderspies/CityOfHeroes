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
		Bhvr	:Huge_Shields_Roman2.bhvr
		Anim	CustomShield_Roman2_01
	End
End

#############################################################

End