#############################################################
## Blood Widow "Poison Dart" FX
#############################################################

FxInfo

LifeSpan 400

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	Behaviors\projectile3.bhvr
		Geom	Arachnos_Blade_Shooting
		Sound widow_training_poison_dart_fire_2 80 80 1
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End