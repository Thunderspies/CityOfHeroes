#############################################################
## GCWormHoleHit.fx
#############################################################

FxInfo
Lifespan 350

#############################################################

Input
	Inpname	Hips
End

#############################################################

####################################################
#################### BODY GLOW 1 ###################
####################################################


Condition

	On	Time
	Time	20

	Event
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End


	Event
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part1	:GCBodyGlow.part
		Pmagnet Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End


	Event
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		part1	:GCBodyGlow.part
		Pmagnet LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End


	Event
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part1	:GCBodyGlow.part
		Pmagnet LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		part1	:GCBodyGlow.part
		Pmagnet LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		part1	:GCBodyGlow.part
		Pmagnet Llegl
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:GCBodyGlow.part
		Pmagnet Llegl
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End



	Event
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		part1	:GCBodyGlow.part
		Pmagnet LlegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:GCBodyGlow.part
		Pmagnet LlegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End


End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Start
		At	Chest
		part1	:GCVortexSteamSmall.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		Pmagnet	ChestNode
		Lifespan 70

	End

	Event
		Ename	GravitationalForce
		Type	Start
		At	Chest

		BhvrOverride
			PositionOffset		0 0 0
			PhysForceType		In
			PhysForceRadius		30
			PhysForcePower		100
			PhysForcePowerJitter	10
		End

		Lifespan	75
	End

	Event
		Ename	LiftForce
		Type	StartPositOnly
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		30
			PhysForcePowerJitter	10
		End

		Lifespan	75

	End
End


################################################################
######################### Vortex In ############################
################################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	Vortex
		Type	Start
		At	Chest
		part1	:GCVortexSteam.part
		Pmagnet	Chest
		Lifespan 80

	End

End

Condition
	On	Time
	Time	20

	Event
		ENAME	Vortex
		Type	Start
		At	Chest
		part	:GCVortexInFast.part
		Lifespan 35

	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Start
		At	Chest
		part	:GCVortexGlows.part
		Pmagnet Chest
		Lifespan 85
	End

End

Condition
	On	Time
	Time	71

	Event
		ENAME	VortexFlash
		Type	Start
		At	Chest
		part1	:GCFlashLrg.part
		Lifespan 20
	End

End

################################################################
######################### Vortex Out ###########################
################################################################

Condition
	On	Time
	Time	82

	Event
		ENAME	VortexFlash
		Type	Start
		At	Chest
		part	:GCFlashLrg.part
		Lifespan 20
	End

End

Condition
	On	Time
	Time	80

	Event
		ENAME	ChestNode
		Type	Start
		At	Chest
		LifeSpan 100
		Sound gravityhit 100 100 .9
	End

	Event
		ENAME	Vortex
		Type	Start
		At	Chest
		part1	:GCVortexSteamOut.part
		Pmagnet	ChestNode
		Lifespan 100
	End

	Event
		Ename	ExpulsionForce
		Type	Start
		At	Chest

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		200
			PhysForcePowerJitter	10
		End

		Lifespan	5
	End

	Event
		Ename	ExpulsionForce
		Type	Start
		At	Chest

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		32
			PhysForcePower		32
			PhysForcePowerJitter	10
		End

		Lifespan	80
	End

End

Condition
	On	Time
	Time	80

	Event
		ENAME	Vortex
		Type	Start
		At	Chest
		part	:GCVortexOutFast.part
		Lifespan 80
	End

	Event
		ENAME	Vortex
		Type	Start
		At	Chest
		part1	:GCVortexGlowsOut.part
		Pmagnet ChestNode
		Lifespan 90
	End

End

End

