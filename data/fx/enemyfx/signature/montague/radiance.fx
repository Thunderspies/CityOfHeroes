#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo
Lifespan 210

Flags	DontSuppress

#############################################################

###	BODY EFFECTS

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Radiance_01 100 100 .8
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	Neck
		Lifespan	30
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	Chest
		Lifespan	30
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	LarmR
		Lifespan	30
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	LarmR
		Lifespan	30
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	LarmL
		Lifespan	30
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	LarmL
		Lifespan	30
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	Ulegl
		Lifespan	30
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	Llegl
		Lifespan	30
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	UlegR
		Lifespan	30
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:BodyGlow01.part
		POther	LlegR
		Lifespan	30
	End

End


###	FLARE OUT

Condition
	On 	Time
	Time 	22
	Event
		Type	Local
		At	Root
		bhvrOverride
			PositionOffset 0 1.3 0
		End
		part	:Glow_Burst_Light.part
		part	:Glow_Burst_Light_base.part
		part	:Shockwave_Light.part
		part	:Shockwave_Light_base.part
		Lifespan 60
	End
End

##########################	SCREEN EFFECT

Condition
	On	Time
	Time	25

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Clouds_Border_Light.part
		Lifespan 150
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Filter_Light.part
		Lifespan 150
	End
End

#############################################################

End
