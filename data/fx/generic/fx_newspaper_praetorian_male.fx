#############################################################
## FX_Newspaper_Praetorian_Female.fx
#############################################################

FxInfo

Flags InheritAnimScale

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prop
		Type	Local
		At	Root
		Anim	FX_Newspaper_Praetorian_Male
	End
End

#############################################################

End