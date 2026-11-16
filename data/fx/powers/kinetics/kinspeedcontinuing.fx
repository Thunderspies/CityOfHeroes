#########################################################
## Kinetics - Siphon Speed Continuing FX
#########################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		part1	:KinEludeBodyStreaks01.part
		#POther	Neck
	End

	Event
		Type	Local
		At	Head
		part1	:KinEludeBodyStreaks01.part
		#POther	Hair
	End

	Event
		Type	Local
		At	SpadR
		part1	:KinEludeBodyStreaks01.part
		#POther	LarmR
	End

	Event
		Type	Local
		At	SpadL
		part1	:KinEludeBodyStreaks01.part
		#POther	LarmL
	End

	Event
		Type	Local
		At	LlegL
		part1	:KinEludeBodyStreaks01.part
		#POther	Ulegl
	End

	Event
		Type	Local
		At	LlegR
		part1	:KinEludeBodyStreaks01.part
		#POther	UlegR
	End
End

## BODY GLOW 2 #######################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	SpinNode
		Type	Local
		At	Chest
		Bhvr	:KineticScaleSmall.bhvr
	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		Bhvr	:KineticSpin1.bhvr
	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		Bhvr	:KineticSpin2.bhvr
	End
End

Condition
	On	Cycle
	Time	30

	Event
		ENAME	Flyaways1
		Type	Posit
		At	Swirl1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:KineticSSSwirlFlyAway01.part
		Lifespan	50
	End
End

Condition
	On	Cycle
	Time	25

	Event
		ENAME	Flyaways2
		Type	Posit
		At	Swirl2
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:KineticSSSwirlFlyAway01.part
		Lifespan	50
	End
End

#########################################################

End