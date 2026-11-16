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
		Bhvr	:Female_Shield_Ascendant1.bhvr
		Anim	CustomShield_Ascendant1_01
	End
End

#############################################################

End