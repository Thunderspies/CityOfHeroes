#############################################################
## Blood Widow "Poison Dart" FX
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	TargetLookAt
		Type	Start
		At	Chest
		Geom	RootToChestAdjustment
		LookAt	Target
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	TargetOffsetR1
		Type	Local
		At	TargetLookAt
		AltPiv	1
		BhvrOverride
			PositionOffset		1.0 1.5 50.0
			StartJitter		1.0 1.0 1.0
		End
	End

	Event
		EName 	TargetOffsetR2
		Type	Local
		At	TargetLookAt
		AltPiv	1
		BhvrOverride
			PositionOffset		7.0 0.0 49.0
			StartJitter		1.0 1.0 1.0
		End
	End

	Event
		EName 	TargetOffsetR3
		Type	Local
		At	TargetLookAt
		AltPiv	1
		BhvrOverride
			PositionOffset		13.0 -1.5 48.0
			StartJitter		1.0 1.0 1.0
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	TargetOffsetL1
		Type	Local
		At	TargetLookAt
		AltPiv	1
		BhvrOverride
			PositionOffset		-1.0 1.5 50.0
			StartJitter		1.0 1.0 1.0
		End
	End

	Event
		EName 	TargetOffsetL2
		Type	Local
		At	TargetLookAt
		AltPiv	1
		BhvrOverride
			PositionOffset		-7.0 0.0 49.0
			StartJitter		1.0 1.0 1.0
		End
	End

	Event
		EName 	TargetOffsetL3
		Type	Local
		At	TargetLookAt
		AltPiv	1
		BhvrOverride
			PositionOffset		-13.0 -1.5 48.0
			StartJitter		1.0 1.0 1.0
		End
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start
		At	WepR
		Bhvr	V_COV\Powers\BloodWidow\Claws\DartBurst_SpikeProjectiles.bhvr
		Geom	Arachnos_Blade_Shooting
		Magnet	TargetOffsetR1
		LookAt	TargetOffsetR1
	End
End

Condition
	On 	Time
	Time 	1

	Event
		EName 	Prime
		Type	start
		At	WepR
		Bhvr	V_COV\Powers\BloodWidow\Claws\DartBurst_SpikeProjectiles.bhvr
		Geom	Arachnos_Blade_Shooting
		Magnet	TargetOffsetR2
		LookAt	TargetOffsetR2
	End
End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Prime
		Type	start
		At	WepR
		Bhvr	V_COV\Powers\BloodWidow\Claws\DartBurst_SpikeProjectiles.bhvr
		Geom	Arachnos_Blade_Shooting
		Magnet	TargetOffsetR3
		LookAt	TargetOffsetR3
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	V_COV\Powers\BloodWidow\Claws\DartBurst_SpikeProjectiles.bhvr
		Geom	Arachnos_Blade_Shooting
		Magnet	TargetOffsetL1
		LookAt	TargetOffsetL1
	End
End

Condition
	On 	Time
	Time 	1

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	V_COV\Powers\BloodWidow\Claws\DartBurst_SpikeProjectiles.bhvr
		Geom	Arachnos_Blade_Shooting
		Magnet	TargetOffsetL2
		LookAt	TargetOffsetL2
	End
End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	V_COV\Powers\BloodWidow\Claws\DartBurst_SpikeProjectiles.bhvr
		Geom	Arachnos_Blade_Shooting
		Magnet	TargetOffsetL3
		LookAt	TargetOffsetL3
	End
End
#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End
End

#############################################################

End