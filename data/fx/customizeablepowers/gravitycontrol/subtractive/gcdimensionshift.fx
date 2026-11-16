#############################################################
## GCDimensionShift.fx
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
		Sound gravityzap2 80 80 .75
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
	Time	0

	Event
		ENAME	HandLOrbit
		Type	Local
		At	WepL
		part1	:GCHandOrbits.part
		Pmagnet WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 40

	End

End

End