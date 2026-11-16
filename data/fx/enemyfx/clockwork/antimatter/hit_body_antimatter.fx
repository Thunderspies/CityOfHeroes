#############################################################
## Hit_Body_Antimatter.fx
#############################################################

FxInfo

Lifespan 120

#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
		Sound eleccage_loop 70 70 .25
	End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet Chest
		POther  Chest
		Lifespan	30
		LifespanJitter	5
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet LarmR
		Lifespan	30
		LifespanJitter	5
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet LarmL
		Lifespan	30
		LifespanJitter	5
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan	30
		LifespanJitter	5
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	30
		LifespanJitter	5
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet Neck
		POther  Neck
		Lifespan	30
		LifespanJitter	5
	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet Hips
		POther  Hips
		Lifespan	30
		LifespanJitter	5
	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet UArmL
		POther  UArmL
		Lifespan	30
		LifespanJitter	5
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet UArmR
		POther  UArmR
		Lifespan	30
		LifespanJitter	5
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet LlegL
		POther  LlegL
		Lifespan	30
		LifespanJitter	5
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan	30
		LifespanJitter	5
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet FootR
		POther  FootR
		Lifespan	30
		LifespanJitter	5
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet FootL
		POther  FootL
		Lifespan	30
		LifespanJitter	5
	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet FootR
		Lifespan	30
		LifespanJitter	5
	End

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet FootL
		Lifespan	30
		LifespanJitter	5
	End
End



End