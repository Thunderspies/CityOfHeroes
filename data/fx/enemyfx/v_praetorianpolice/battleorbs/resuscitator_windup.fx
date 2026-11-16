#############################################################
## PlasmaticStunner_Attack.fx
#############################################################

FxInfo
LifeSpan 90

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

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	5
		Part	:Muzzle_Glow_Resuscitator_Windup.part
		POther	TargetAnchor
		Lifespan 60
	End
End

Condition
	On	Cycle
	Time	7

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	5
		Part	:Muzzle_Flash_Resuscitator_Windup.part
		POther	TargetAnchor
		Lifespan 30
	End
End

Condition
	On	Time
	Time	35

	Event
		Ename	Flash
		Type	Local
		At	TargetAnchor
		Part	:Beam_Glow_Small.part
		POther	MkIIIOffsets_5
		Lifespan 30
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