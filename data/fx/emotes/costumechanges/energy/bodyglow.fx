#############################################################
## BodyGlow.fx
#############################################################

FxInfo
Lifespan 80

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet Head
		Lifespan	50
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet UarmR
		Lifespan	70
	End



	Event
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet UarmL
		Lifespan	70
	End


	Event
		ENAME	ChestGlow
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	Chest
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet Hips
		POther  Hips
		Lifespan	80
	End

	Event
		ENAME	ChestGlow
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	Hips
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet UarmL
		POther  UarmL
		Lifespan	50
	End



	Event
		ENAME	RightLarmGlow
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	WepR
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan	50
	End


	Event
		ENAME	LeftLarmGlow
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	WepL
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	50
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	UlegL
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan	50
	End


	Event
		ENAME	RightULegGlow
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	UlegR
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan	50
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet FootR
		POther  FootR
		Lifespan	50
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet FootL
		POther  FootL
		Lifespan	50
	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet LLegR
		Lifespan	50
	End

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow.part
		Part	:Glow_Linger.part
		Pmagnet LLegL
		Lifespan	50
	End


End



End