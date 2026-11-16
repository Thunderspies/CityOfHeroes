#############################################################
## Placate_Activate.fx
#############################################################

FxInfo
LifeSpan	60

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Hide2 70 70 .7
	End
End

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Hide2_loop 70 70 .5
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 200
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	5

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part

		POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part
		POther Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part
		POther Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part
		POther UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Part	:Body_WhiteNoise.part
		Part	:Body_WhiteNoise_Swirl.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End
End

############################################################################################################


Condition
	On	Cycle
	Time	30

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Part	:Body_WhiteNoise.part
		POther 	Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Body_WhiteNoise.part
		POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Part	:Body_WhiteNoise.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Body_WhiteNoise.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Part	:Body_WhiteNoise.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Body_WhiteNoise.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		Part	:Body_WhiteNoise.part
		POther 	Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Part	:Body_WhiteNoise.part
		POther 	Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		Part	:Body_WhiteNoise.part
		POther 	UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Part	:Body_WhiteNoise.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30

	End
End

############################################################################################################

End