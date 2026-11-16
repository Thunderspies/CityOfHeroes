#############################################################
## Smash_Activate.fx
#############################################################

FxInfo
LifeSpan 60

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	PlasmaAnchorR
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	3.2 0.7 0.0
		End
	End
End


Condition
	On	Time
	Time	0

	Event
		EName 	Prime
		Type	Start
		At	PlasmaAnchorR
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	1.0 1.5 1.0
			TrackRate		4.0
		End
		Part	:Projectile_Head_Ring.part
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	Time
	Time	1

	Event
		EName 	Prime2
		Type	Start
		At	PlasmaAnchorR
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	1.0 1.5 1.0
			TrackRate		4.0
		End
		Part	:Projectile_Head_Ring.part
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	Time
	Time	2

	Event
		EName 	Prime3
		Type	Start
		At	PlasmaAnchorR
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	1.0 1.5 1.0
			TrackRate		4.0
		End
		Part	:Projectile_Head_Ring.part
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	Time
	Time	3

	Event
		EName 	Prime4
		Type	Start
		At	PlasmaAnchorR
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	1.0 1.5 1.0
			TrackRate		4.0
		End
		Part	:Projectile_Head_Ring.part
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	Time
	Time	4

	Event
		EName 	Prime5
		Type	Start
		At	PlasmaAnchorL
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	1.0 1.5 1.0
			TrackRate		4.0
		End
		Part	:Projectile_Head_Ring.part
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	Time
	Time	5

	Event
		EName 	Prime6
		Type	Start
		At	PlasmaAnchorR
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	1.0 1.5 1.0
			TrackRate		4.0
		End
		Part	:Projectile_Head_Ring.part
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End


Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
	End
End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy
	End
End

Condition
	On 	Prime5Hit

	Event
		EName 	Prime5
		Type	Destroy
	End
End

Condition
	On 	Prime6Hit

	Event
		EName 	Prime6
		Type	Destroy
	End
End

#############################################################

End
