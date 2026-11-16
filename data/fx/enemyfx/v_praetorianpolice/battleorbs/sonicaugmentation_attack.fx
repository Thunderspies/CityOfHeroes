#############################################################
## MonitorSphere_Combat.fx
#############################################################

FxInfo
Lifespan 60

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MkIIOffsets
		Type	Local
		At	Chest
		BhvrOverride
			Alpha	1
		End
		Geom	FX_BattleOrb_MkII_Offsets
	End

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	7
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tactics_Rings.part
		Part	:Tactics_Glow.part
		Part	:Tactics_Glow_Back.part
	End

End

#########################################################################################


End


