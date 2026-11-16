#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	11

	Event
		EName	MkIIOffsets
		Type	Local
		At	Origin
		Geom	FX_BattleOrb_MkII_Offsets
		sound Plasma_Blast 200 200 .8
	End

	Event
		Ename	Flash
		Type	Local
		At	MkIIOffsets
		AltPiv	1
		Part	:Muzzle_Flash_Small.part
		Part	:Muzzle_Flash_Circle.part
	End

	Event
		Ename 	Prime
		Type	Start
		At	MkIIOffsets
		AltPiv	1
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0
		End
		Part	:Projectile_Trail_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Part	:Projectile_Trail_Glow_Small.part
		Magnet	Target
		LookAt	Target
		Lifespan 40
	End
End

Condition
	On	Time
	Time	13

	Event
		Ename	Flash
		Type	Local
		At	MkIIOffsets
		AltPiv	4
		Part	:Muzzle_Flash_Small.part
		Part	:Muzzle_Flash_Circle.part
	End

	Event
		Ename 	Prime1
		Type	Start
		At	MkIIOffsets
		AltPiv	4
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0
		End
		Part	:Projectile_Trail_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Part	:Projectile_Trail_Glow_Small.part
		Magnet	Target
		LookAt	Target
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	16

	Event
		Ename	Flash
		Type	Local
		At	MkIIOffsets
		AltPiv	3
		Part	:Muzzle_Flash_Small.part
		Part	:Muzzle_Flash_Circle.part
	End

	Event
		Ename 	Prime2
		Type	Start
		At	MkIIOffsets
		AltPiv	3
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0

		End
		Part	:Projectile_Trail_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Part	:Projectile_Trail_Glow_Small.part
		Magnet	Target
		LookAt	Target
		Lifespan 40
	End
End

Condition
	On	Time
	Time	19

	Event
		Ename	Flash
		Type	Local
		At	MkIIOffsets
		AltPiv	6
		Part	:Muzzle_Flash_Small.part
		Part	:Muzzle_Flash_Circle.part
	End

	Event
		Ename 	Prime3
		Type	Start
		At	MkIIOffsets
		AltPiv	6
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0

		End
		Part	:Projectile_Trail_Ember_Small.part
		Part	:Projectile_Trail_Tendril_Small.part
		Part	:Projectile_Trail_Glow_Small.part
		Magnet	Target
		LookAt	Target
		Lifespan 40
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
	On 	Prime1Hit

	Event
		EName 	Prime1
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

#############################################################

End