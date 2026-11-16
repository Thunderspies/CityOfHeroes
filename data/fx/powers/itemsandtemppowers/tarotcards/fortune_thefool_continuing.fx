#############################################################
## Fortune_TheFool_Continuing.fx
#############################################################

FxInfo

## PERSISTENT FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	HookupSpin
		Type	PositOnly
		At	Chest
		BhvrOverride
			Spin		0 -0.04 0
		End
		Geom	TarotCardRig
	End
End

Condition
	On	Cycle
	Time	105
	Chance	0.5

	Event
		Ename	Card1
		Type	Local
		At	HookupSpin
		AltPiv	1
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Continuing_TarotCard_TheFool.part
		Sound Tarot_Persistent_01 10 10 .2
		Sound Tarot_Persistent_02 10 10 .2
		Sound Tarot_Persistent_03 10 10 .2
		Lifespan 10
	End


	Event
		Ename	Card2
		Type	Local
		At	HookupSpin
		AltPiv	2
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Continuing_TarotCard_TheFool.part
		Lifespan 10
	End


	Event
		Ename	Card3
		Type	Local
		At	HookupSpin
		AltPiv	3
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Continuing_TarotCard_TheFool.part
		Lifespan 10
	End


End

### ON DEATH ###########################################################
#
#Condition
#	On	Death
#
#	Event
#		Type	Local
#		At	Hips
#		Part	V_COV\EnemyPowers\Arachnos\ScorpionShield\ERing01.part
#		Part	V_COV\EnemyPowers\Arachnos\ScorpionShield\Cloud01.part
#		lifespan 1
#	End
#End

#############################################################

End