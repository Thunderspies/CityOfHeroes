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
		Bhvr	:Female_Shield_Tribal1.bhvr
		Anim	CustomShield_Tribal1_Emblem1
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	:Female_Shield_Tribal1.bhvr
		Anim	CustomShield_Tribal1_Emblem2
	End
End

#############################################################

End