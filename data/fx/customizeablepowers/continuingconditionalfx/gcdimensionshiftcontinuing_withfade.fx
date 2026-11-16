#############################################################
## GCDimensionShiftContinuing_WithFade.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	75

	Event
		Type	Local
		At 	origin
		Sound glow_loop 80 80 0.45
	End

	Event
		Type	Local
		At 	origin
		Sound glow6_loop 80 80 0.25
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Alpha
		Type	Local
		At	Origin
		Bhvr	POWERS\GravityControl\DimensionShift.bhvr
		Flags	AdoptParentEntity
	End

	Event
		ENAME	Vortex
		Type	Local
		At	Hips
		part1	:GCVortexSteamSmall.part
		LifeSpan 80
	End
End

Condition
	On	Time
	Time	5

	Event
		ENAME	Vortex
		Type	Local
		At	Hips
		part	:GCVortexSmall.part
		LifeSpan 60
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	Vortex
		Type	Local
		At	Hips
		part	:GCVortexGlowsSmall.part
		Sound gravityhit 80 80 0.8
		Pmagnet Hips
		LifeSpan 80
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet Neck
		Lifespan 0
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:GCBodyGlow.part
		Pmagnet Chest
		Lifespan 0
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:GCBodyGlow.part
		Pmagnet LarmR
		Lifespan 0
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:GCBodyGlow.part
		Pmagnet LarmR
		Lifespan 0
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:GCBodyGlow.part
		Pmagnet LarmL
		Lifespan 0
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:GCBodyGlow.part
		Pmagnet LarmL
		Lifespan 0
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:GCBodyGlow.part
		Pmagnet Llegl
		Lifespan 0
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:GCBodyGlow.part
		Pmagnet Llegl
		Lifespan 0
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:GCBodyGlow.part
		Pmagnet LlegR
		Lifespan 0
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:GCBodyGlow.part
		Pmagnet LlegR
		Lifespan 0
	End
End

#########################################################

End

