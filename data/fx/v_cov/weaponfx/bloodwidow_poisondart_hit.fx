#############################################################
## Blood Widow "Poison Dart" Hit FX
#############################################################

FxInfo

LifeSpan 105

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type 	Local
		At	chest
		geom	Arachnos_Blade_Shooting
		Sound arrowhit 50 50 .67
	End
End

#############################################################

End