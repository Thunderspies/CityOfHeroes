#############################################################
## GCWormHole.fx
#############################################################

FxInfo

Lifespan 120

#############################################################

Condition
	On	Time
	Time	25

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowSlow.part
		Pmagnet Neck
		Lifespan 90
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowSlow.part
		Pmagnet Chest
		Lifespan 90
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowSlow.part
		Pmagnet LarmR
		Lifespan 90
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowSlow.part
		Pmagnet LarmR
		Lifespan 90
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowSlow.part
		Pmagnet LarmL
		Lifespan 90
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowSlow.part
		Pmagnet LarmL
		Lifespan 90
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowMist.part
		Sound gravityzap5 100 100 0.85
		Pmagnet Neck
		Lifespan 0
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowMist.part
		Pmagnet Chest
		Lifespan 0
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowMist.part
		Pmagnet LarmR
		Lifespan 0
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowMist.part
		Pmagnet LarmR
		Lifespan 0
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	:GCBodyGlowMist.part
		Pmagnet LArmL
		Lifespan 0
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlowMist.part
		Pmagnet LArmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 0
	End
End

#############################################################

End