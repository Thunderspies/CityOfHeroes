#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	25

	Event
		EName	Ring
		Type	Local
		At	WepL
		Sound PPD_ForceNetting_Attack_01 200 200 1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare02.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Glow01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Ring01.part
		Lifespan 55
	End
End

#############################################################

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	BendMystic
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MGlow.part
		Lifespan 55
	End

	Event
		Type	Local
		At	BendMystic
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		90 0 0
		End
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BeamRings01.part
		Lifespan 45
	End

	Event
		Type	Local
		At	BendMystic
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0 25
		End
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MAidBeam01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MAidBeam02.part
		pOther	WepL
		Lifespan 55
	End
End

#############################################################

End