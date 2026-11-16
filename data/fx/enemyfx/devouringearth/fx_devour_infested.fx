#########################################################
##	jack
##
FxInfo


Input
	InpName	Neck
End

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Neck
		Anim	FX_Devour_Infested
	End


End

End