#############################################################
## Blood Widow "Poison Dart" Hit FX
#############################################################

FxInfo

ClampMaxScale	1.25 1.25 1.25
ClampMaxScale	0.75 0.75 0.75

LifeSpan 105

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Create
		Inherit	ALL
		Update	Position Rotation
		At	Chest
		geom	Arachnos_Blade_Shooting
		Sound widow_training_dart_impact 50 50 .7
	End

	Event
		Type	Local
		At	Root
		ChildFX	:Child_PoisonHit_Small.fx
	End
End

#############################################################

End