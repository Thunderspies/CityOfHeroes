#############################################################
## PlasmaticStunner_Attack.fx
#############################################################

FxInfo
LifeSpan 60

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
	End


	Event
		EName	TargetAnchor
		Type	PositOnly
		At	T_Chest
		LookAt	MkIIIOffsets_5
	End

	Event
		Ename	MkIIIOffsets_5
		Type	Local
		At	MkIIIOffsets
		AltPiv	5
	End

End

Condition
	On	Time
	Time	5



	Event
		Ename	Flash
		Type	Local
		At	TargetAnchor
		Part	:Beam_Glow_Small.part
		POther	MkIIIOffsets_5
		Lifespan 30
	End

	Event
		Ename	Flash
		Type	Local
		At	MkIIIOffsets
		AltPiv	5
		Part	:Muzzle_Flash_Resuscitator.part
		POther	TargetAnchor
		Lifespan 30
	End

End

Condition
	On	Time
	Time	20

	Event
		Ename	Flash
		Type	Local
		At	MkIIIOffsets_5
		Part	:Beam_Core_Resuscitator.part
		Part	:Beam_Glow_Resuscitator.part
		POther	TargetAnchor
		Lifespan 20
	End

	Event
		Ename 	Prime
		Type	Start
		At	MkIIIOffsets_5
		BhvrOverride
			TrackRate		6.0
		End
		Part	:Projectile_Head_Resuscitator.part
		Magnet	TargetAnchor
		LookAt	TargetAnchor
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

#############################################################

End