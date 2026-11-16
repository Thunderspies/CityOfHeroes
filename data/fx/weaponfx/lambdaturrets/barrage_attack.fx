#############################################################
## Turret_MuzzleFlash.fx
#############################################################

FxInfo
LifeSpan 50

#############################################################
Condition
	On	Time
	Time	0

	Event
		EName	MuzzleAnchor
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 2.5 51.0
			PyrRotate	90 270 0
		End
		Sound LambdaTurret_Barrage_Attack_01 300 300 .8
		Lifespan	30
	End
End

Condition
	On 	Cycle
	Time 	5

	Event
		Type	Local
		At	MuzzleAnchor
		Part	:Flash_Beam.part
		Part	:Flash_Beam_Highlight.part
		Part	:Flash_Beam_Smoke.part
		Part	:Flash_Beam_light.part
		Lifespan	12
	End

	Event
		Type	Local
		At	MuzzleAnchor
		Part	:Flash_Circle.part
		Part	:Flash_Circle_Large.part
		Part	:Flash_Circle_Fire.part
		Lifespan	2
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		EName 	Prime
		Type	Start
		At	MuzzleAnchor
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		10.0
		End
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Head_Ember_Highlight.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName 	Prime2
		Type	Start
		At	MuzzleAnchor
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		10.0
		End
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Head_Ember_Highlight.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName 	Prime3
		Type	Start
		At	MuzzleAnchor
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		10.0
		End
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Head_Ember_Highlight.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		EName 	Prime4
		Type	Start
		At	MuzzleAnchor
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		10.0
		End
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Head_Ember_Highlight.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	Time
	Time	25

	Event
		EName 	Prime5
		Type	Start
		At	MuzzleAnchor
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		10.0
		End
		Part	:Projectile_Head_Ember_Small.part
		Part	:Projectile_Head_Ember_Highlight.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		EName	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		EName	Prime3
		Type	Destroy
	End
End

Condition
	On	Prime4Hit

	Event
		EName	Prime4
		Type	Destroy
	End
End

Condition
	On	Prime5Hit

	Event
		EName	Prime5
		Type	Destroy
	End
End

#############################################################

End