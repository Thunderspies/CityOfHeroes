#############################################################
## Costume Anim Entity - Shield - Spartan 1
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
		Bhvr	:Female_Shields_Spartan1.bhvr
		Anim	CustomShield_Spartan1_02
End

#############################################################

End