#############################################################
## Costume Anim Entity - Shield - Roman 1
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume IsShield

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	:Male_Shields_Round1.bhvr
		Anim	CustomShield_Round1_Back
	End
End

#############################################################

End