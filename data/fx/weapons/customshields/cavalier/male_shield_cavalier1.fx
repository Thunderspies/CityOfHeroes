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
		Bhvr	:Male_Shield_Cavalier1.bhvr
		Anim	CustomShield_Cavalier1_01
	End
End

#############################################################

End