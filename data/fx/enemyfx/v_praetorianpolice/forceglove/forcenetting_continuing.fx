#############################################################
## GlowBody.fx
#############################################################

FxInfo

## FORM SHIELD MATRIX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  Hair
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  UarmR
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  LarmL
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  WepR
	End

	Event
		Type	Local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  WepL
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\UlegGlow01.part
		POther  Llegl
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\UlegGlow01.part
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  FootL
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  ToeR
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceNetting\BodyGlow01.part
		POther  ToeL
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootL
		ChildFX	ENEMYFX\V_PraetorianPolice\ForceGlove\Child_ForceNetting_Pulse.fx
		Lifespan 30
	End
End

Condition
	On	Cycle
	Time	183

	Event
		Type	Local
		At	FootL
		ChildFX	ENEMYFX\V_PraetorianPolice\ForceGlove\Child_ForceNetting_Pulse.fx
		Lifespan 30
	End
End

#############################################################

End