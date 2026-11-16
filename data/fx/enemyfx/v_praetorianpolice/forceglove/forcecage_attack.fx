#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	WepL
		Sound PPD_ForceCage_Attack_01 200 200 1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare02.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Glow01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Ring01.part
		Lifespan 55
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	WepR
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
	Time	20

	Event
		Ename	Offset_WepL
		Type	Posit
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MGlow.part
		LookAt	Target
		Lifespan 45
	End

	Event
		Type	Start
		At	Offset_WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		0 0 90
		End
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BeamRings01.part
		Lifespan 35
	End

	Event
		Type	Local
		At	Offset_WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0 25
		End
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MAidBeam01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MAidBeam02.part
		pOther	WepL
		Lifespan 45
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	Offset_WepR
		Type	Posit
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MGlow.part
		LookAt	Target
		Lifespan 45
	End

	Event
		Type	Start
		At	Offset_WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		0 0 -90
		End
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BeamRings01.part
		Lifespan 35
	End

	Event
		Type	Local
		At	Offset_WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0 25
		End
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MAidBeam01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\MAidBeam02.part
		pOther	WepR
		Lifespan 45
	End
End

#############################################################

End