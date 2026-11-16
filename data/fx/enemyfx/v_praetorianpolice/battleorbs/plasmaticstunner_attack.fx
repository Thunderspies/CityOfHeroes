#############################################################
## PlasmaticStunner_Attack.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	MkIIIOffsets
		Type	Local
		At	Chest
		BhvrOverride
			Alpha	1
		End
		Geom	FX_BattleOrb_MkIII_Offsets
		Sound IDF_BattleOrb_PlasmaStunner_Attack_01 200 200 .8
	End
End

#Condition
#	On	Time
#	Time	15
#
#	Event
#		Type	Local
#		At	Root
#		sound Antimatter_Ray 200 200 .8
#	End
#End

Condition
	On	Time
	Time	5

	Event
		Ename	Flash
		Type	Local
		At	MkIIIOffsets
		AltPiv	1
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan 20
	End

	Event
		Ename	Flash
		Type	Local
		At	MkIIIOffsets
		AltPiv	2
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan 20
	End

	Event
		Ename	Flash
		Type	Local
		At	MkIIIOffsets
		AltPiv	3
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan 20
	End

	Event
		Ename	Flash
		Type	Local
		At	MkIIIOffsets
		AltPiv	4
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan 20
	End
End

#############################################################

Condition
	On	Time
	Time 	5

	Event
		Ename	TargetAnchor
		Type	Start
		At	T_Chest
	End

	Event
		EName 	BeamGeo
		Type	Start
		At	MkIIIOffsets
		AltPiv	1
		Bhvr	behaviors/LightningBeam.bhvr
		BhvrOverride
			StartColor	255 0 0
		End
		Part	:Beam_Glow.part
		Part	:Muzzle_Buildup_Flash_Alpha.part
		Part	:Muzzle_Buildup_Rings_Alpha.part

		geom	FX_ElectricBeam_Arc
		Magnet	TargetAnchor
		LookAt	TargetAnchor
		POther	TargetAnchor
		LifeSpan		20
	End


	Event
		Ename 	Prime
		Type	Start
		At	MkIIIOffsets
		AltPiv	1
		BhvrOverride
			TrackRate		6.0
		End

		Part	:Projectile_Trail_Ember_Small_Antimatter.part
		Part	:Projectile_Trail_Tendril_Small_Subtractive.part
		Part	:Projectile_Trail_Glow_Small_Subtractive.part

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		Lifespan 40
	End
End

Condition
	On	Time
	Time 	7

	Event
		Ename	TargetAnchor
		Type	Start
		At	T_Chest
	End

	Event
		EName 	BeamGeo
		Type	Start
		At	MkIIIOffsets
		AltPiv	2
		Bhvr	behaviors/LightningBeam.bhvr

		Part	:Beam_Glow.part
		Part	:Muzzle_Buildup_Flash_Alpha.part
		Part	:Muzzle_Buildup_Rings_Alpha.part

		geom	FX_ElectricBeam_Arc

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		POther	TargetAnchor

		LifeSpan		20
	End


	Event
		Ename 	Prime1
		Type	Start
		At	MkIIIOffsets
		AltPiv	2
		BhvrOverride
			TrackRate		6.0
		End

		Part	:Projectile_Trail_Ember_Small_Antimatter.part
		Part	:Projectile_Trail_Tendril_Small_Subtractive.part
		Part	:Projectile_Trail_Glow_Small_Subtractive.part

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		Lifespan 40
	End
End

Condition
	On	Time
	Time 	9

	Event
		Ename	TargetAnchor
		Type	Start
		At	T_Chest
	End

	Event
		EName 	BeamGeo
		Type	Start
		At	MkIIIOffsets
		AltPiv	3
		Bhvr	behaviors/LightningBeam.bhvr

		Part	:Beam_Glow.part
		Part	:Muzzle_Buildup_Flash_Alpha.part
		Part	:Muzzle_Buildup_Rings_Alpha.part

		geom	FX_ElectricBeam_Arc

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		POther	TargetAnchor

		LifeSpan		20
	End


	Event
		Ename 	Prime2
		Type	Start
		At	MkIIIOffsets
		AltPiv	3
		BhvrOverride
			TrackRate		6.0
		End

		Part	:Projectile_Trail_Ember_Small_Antimatter.part
		Part	:Projectile_Trail_Tendril_Small_Subtractive.part
		Part	:Projectile_Trail_Glow_Small_Subtractive.part

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		Lifespan 40
	End
End

Condition
	On	Time
	Time 	11

	Event
		Ename	TargetAnchor
		Type	Start
		At	T_Chest
	End

	Event
		EName 	BeamGeo
		Type	Start
		At	MkIIIOffsets
		AltPiv	4
		Bhvr	behaviors/LightningBeam.bhvr

		Part	:Beam_Glow.part
		Part	:Muzzle_Buildup_Flash_Alpha.part
		Part	:Muzzle_Buildup_Rings_Alpha.part

		geom	FX_ElectricBeam_Arc

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		POther	TargetAnchor

		LifeSpan		20
	End


	Event
		Ename 	Prime3
		Type	Start
		At	MkIIIOffsets
		AltPiv	4
		BhvrOverride
			TrackRate		6.0
		End

		Part	:Projectile_Trail_Ember_Small_Antimatter.part
		Part	:Projectile_Trail_Tendril_Small_Subtractive.part
		Part	:Projectile_Trail_Glow_Small_Subtractive.part

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		Lifespan 40
	End
End


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