#############################################################
## AimedShot_Hit.fx
#############################################################

FxInfo

LifeSpan 5

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Hit_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Hit_Star01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Hit_Streaks01.part
		Lifespan 5
	End
End

#############################################################

End