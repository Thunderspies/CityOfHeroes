#############################################################
## BodyGlow.fx
#############################################################

FxInfo
Lifespan 200

#########################################################
#################### BODY GLOW 2 ########################
#########################################################



Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Female.part
		Pmagnet LArmR
		POther	LArmR
		Lifespan	60
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Female.part
		Pmagnet WepR
		POther  WepR
		Lifespan	60
	End


	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Feet.part
		Lifespan	60
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Feet.part
		Lifespan	60
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 .25 0.0
		End
		part	:Glow_Female.part
		Lifespan	60
	End

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 1.0 0.0
		End
		part	:Glow_Female.part
		Pmagnet Chest
		POther	Chest
		Lifespan	60
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Female.part
		Pmagnet UArmL
		POther  UArmL
		Lifespan	60
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Female.part
		Pmagnet UArmR
		POther  UArmR
		Lifespan	60
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Female.part
		Pmagnet LLegR
		POther  LLegR
		Lifespan	60
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Female.part
		Pmagnet LLegL
		POther  LLegL
		Lifespan	60
	End


End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Female.part
		Pmagnet LArmL
		POther	LArmL
		Lifespan	45
		LifespanJitter	20
	End


	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Female.part
		Pmagnet WepL
		POther  WepL
		Lifespan	45
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Feet.part
		Lifespan	60
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Glow_Feet.part
		Lifespan	45
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	Chest
		part	:Glow_Female.part
		Pmagnet Hips
		POther  Hips
		Lifespan	47
		LifespanJitter	10
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	Hips
		part	:Glow_Female.part
		Pmagnet LLegL
		POther  LLegL
		Lifespan	47
		LifespanJitter	10
	End

	Event

		Type	Local
		At	LLegL
		part	:Glow_Female.part
		Lifespan	47
		LifespanJitter	10
	End

	Event
		Type	Local
		At	LLegL
		part	:Glow.part
		Pmagnet FootL
		POther  FootL
		Lifespan	47
		LifespanJitter	10
	End

	Event
		Type	Local
		At	FootL
		part	:Glow_Feet.part
		Lifespan	50
		LifespanJitter	10
	End
End

Condition
	On	Time
	Time	8

	Event
		ENAME	RightULegGlow
		Type	Local
		At	Hips
		part	:Glow_Female.part
		Pmagnet LLegR
		POther  LLegR
		Lifespan	48
		LifespanJitter	10
	End

	Event

		Type	Local
		At	LLegR
		part	:Glow_Female.part
		Lifespan	48
		LifespanJitter	10
	End

	Event
		Type	Local
		At	LLegR
		part	:Glow_Female.part
		Pmagnet FootR
		POther  FootR
		Lifespan	48
		LifespanJitter	10
	End

	Event
		Type	Local
		At	FootR
		part	:Glow_Feet.part
		Lifespan	48
		LifespanJitter	10
	End

End

#########################################################

End