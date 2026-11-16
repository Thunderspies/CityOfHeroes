#############################################################
## GCDistortionField.fx
#############################################################

FxInfo
Lifespan 60

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
		Sound teletrans4 80 80 .88
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:GCBodyGlow.part
		Pmagnet Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End



	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:GCBodyGlow.part
		Pmagnet LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:GCBodyGlow.part
		Pmagnet LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:GCBodyGlow.part
		Pmagnet LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlow.part
		Pmagnet LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0
	End

End

#########################################################
################## BODY GLOW MIST #######################
#########################################################



Condition
	On	Time
	Time	0



	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlowMist.part
		Pmagnet Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:GCBodyGlowMist.part
		Pmagnet Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End



	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:GCBodyGlowMist.part
		Pmagnet LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:GCBodyGlowMist.part
		Pmagnet LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0

	End

End

#########################################################
#################### HAND ORBITS ########################
#########################################################


Condition
	On	Time
	Time	20

	Event
		ENAME	Vortex
		Type	Posit
		At	WepL
		part	:GCLiftingElement1out.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 580
	End

End

Condition
	On	Time
	Time	20

	Event
		ENAME	Vortex
		Type	Posit
		At	WepR
		part	:GCLiftingElement1out.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 580
	End

End

End