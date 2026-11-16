#############################################################
## Costume Anim Entity - Shield - Roman 1
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Anim	CustomShield_Round1_Back
	End
End

#############################################################

End