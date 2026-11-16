#############################################################
## Blood Widow "Poison Dart" FX
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	26

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	Behaviors\projectile3.bhvr
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Sound1
		Type	Start
		At	Chest
		Sound widow_training_dart_burst_fire_2 80 80 1
	End
End
#############################################################

Condition
	On 	Time
	Time 	26

	Event
		Type	Start
		At	WepR
		ChildFX	:Child_SpikeProjectile.fx
	End
End

Condition
	On 	Time
	Time 	32

	Event
		Type	Start
		At	WepR
		ChildFX	:Child_SpikeProjectile.fx
	End
End

Condition
	On 	Time
	Time 	38

	Event
		Type	Start
		At	WepR
		ChildFX	:Child_SpikeProjectile.fx
	End
End

Condition
	On 	Time
	Time 	44

	Event
		Type	Start
		At	WepR
		ChildFX	:Child_SpikeProjectile.fx
	End
End

Condition
	On 	Time
	Time 	50

	Event
		Type	Start
		At	WepR
		ChildFX	:Child_SpikeProjectile.fx
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