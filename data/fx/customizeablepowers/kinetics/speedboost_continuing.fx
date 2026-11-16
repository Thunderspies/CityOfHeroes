#########################################################
## Kinetics - Siphon Speed Continuing FX
#########################################################

FxInfo

#Lifespan 200

## SOUND FX #######################################################

#Condition
#	On	Time
#	Time	0
#	
#
#	Event
#		Type	Local
#		At 	Origin
#		Sound	kin2_loop 80 60 .22
#		#bhvr	behaviors/soundFade2.bhvr
#		#Flags	PowerLoopingSound
#	End
#End


## BODY FX #######################################################

Condition
	On	Time
	Time	0
	
	Event	
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Kinetics\KinEludeBodyStreaks01.part
		#POther	Neck
	End

	Event	
		Type	Local
		At	Head
		part1	CustomizeablePowers\Kinetics\KinEludeBodyStreaks01.part
		#POther	Hair
	End

	Event	
		Type	Local
		At	SpadR
		part1	CustomizeablePowers\Kinetics\KinEludeBodyStreaks01.part
		#POther	LarmR
	End

	Event	
		Type	Local
		At	SpadL
		part1	CustomizeablePowers\Kinetics\KinEludeBodyStreaks01.part
		#POther	LarmL
	End
	
	Event	
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Kinetics\KinEludeBodyStreaks01.part
		#POther	Ulegl
	End

	Event	
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Kinetics\KinEludeBodyStreaks01.part
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
		Bhvr	CustomizeablePowers\Kinetics\KineticScaleSmall.bhvr
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin1.bhvr
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin2.bhvr
	End
End

Condition
	On	Cycle
	Time	30

	Event	
		ENAME	Flyaways1
		Type	Posit
		At	Swirl1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\Kinetics\KineticSSSwirlFlyAway01.part
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\Kinetics\KineticSSSwirlFlyAway01.part
		Lifespan	50
	End
End	

#########################################################

End		