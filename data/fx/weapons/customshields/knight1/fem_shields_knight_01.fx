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
		Bhvr	:Fem_Shields_Knight_01.bhvr
		Anim	CustomShield_Knight_01
	End
End

#############################################################

End