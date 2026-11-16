#############################################################
## groceries_l.fx
#############################################################

FxInfo

Flags InheritAlpha

#####  Groceries prop   #######################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Prop
		Type	Local
		At	WepL
		Geom	Prop_Groceries
		Sound PaperBagRustle_Loop 30 30 0.15
	End
End

#############################################################

End
