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
		Bhvr	:Huge_Shield_Ascendant1.bhvr
		Anim	CustomShield_Ascendant2_01
	End
End

#############################################################

End