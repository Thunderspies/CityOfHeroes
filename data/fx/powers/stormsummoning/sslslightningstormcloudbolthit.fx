#############################################################
## SSLSLightningStormCloudBoltHit.fx
#############################################################

FxInfo
Lifespan 30

#########################################################

Input
	Inpname	Hips
End

#########################################################


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound elecfry 100 100 .55

	End

End

#########################################################
###################### FLASH 1 ##########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Flash
		Type	Posit
		At	Chest
		part1	:SSShockedFlash01.Part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 3

	End

End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:SSShockedBodyGlows01.Part
		Pmagnet Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:SSShockedBodyGlows01.Part
		Pmagnet Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:SSShockedBodyGlows01.Part
		Pmagnet LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:SSShockedBodyGlows01.Part
		Pmagnet LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:SSShockedBodyGlows01.Part
		Pmagnet LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:SSShockedBodyGlows01.Part
		Pmagnet LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part1	:SSShockedBodyGlows01.Part
		Pmagnet Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:SSShockedBodyGlows01.Part
		Pmagnet Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part1	:SSShockedBodyGlows01.Part
		Pmagnet LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:SSShockedBodyGlows01.Part
		Pmagnet LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0

	End


End

Condition
	On Time
	Time 0

	Repeat	11

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSparkExplode.fx

		Lifespan	90

	End

End


Condition
	On Time
	Time 0

	Repeat	15

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90

	End

End

End