#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Prop_R
		Type	Local
		At	Root
		Anim	Fx_Cot_Scroll_Narrow
	End
End

#############################################################

End