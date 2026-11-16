#########################################################
## Kinetics - Siphon Speed
#########################################################

FxInfo

Lifespan 200

## SOUND FX #######################################################

Condition
	On	Time
	Time	15
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic2 80 80 .8
	End
End


## CAST FX #######################################################

Condition
	On	Time
	Time	25

	Event	
		ENAME	WepLGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\Kinetics\KineticHandGlows01.part
		Lifespan	20
	End
End

Condition
	On	Time
	Time	20

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	CustomizeablePowers\Kinetics\KineticBodyGlows01.part
		POther	LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	30
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\Kinetics\KineticBodyGlows01.part
		POther	LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 30
	End
End


Condition
	On	Time
	Time	25

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	Target
		part	CustomizeablePowers\Kinetics\KineticIDBodySwirls01.part
		PMagnet LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 20
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	LarmL
		part	CustomizeablePowers\Kinetics\KineticIDBodySwirls01.part
		PMagnet Target
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 20
	End
End

## FLYAWAYS #######################################################

Condition
	On	Time
	Time	20

	Event	
		ENAME	FlyawayNode
		Type	Local
		At	WepL
		Bhvr	CustomizeablePowers\Kinetics\KineticScaleSmall.bhvr
		LifeSpan	40
	End
End
 
Condition
	On	Time
	Time	20

	Event	
		ENAME	Flyaways1
		Type	Start
		At	FlyawayNode
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan	20
	End
End

Condition
	On	Time
	Time	25

	Event	
		ENAME	Flyaways2
		Type	Start
		At	FlyawayNode
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan	20
	End
End

Condition
	On	Time
	Time	30

	Event	
		ENAME	Flyaways3
		Type	Start
		At	FlyawayNode
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan	20
	End

End

### CAST 2 #######################################################
#
#Condition
#	On	Time
#	Time	80
#
#	Event	
#		ENAME	WepLGlow
#		Type	Local
#		At	WepL
#		part	:KineticHandGlowsCyan01.part
#		Lifespan 35
#	End
#End
#
#Condition
#	On	Time
#	Time	75
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	SpadL
#		part	:KineticBodyGlowsCyan01.part
#		POther LarmL
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 40
#	End
#	
#	Event	
#		ENAME	LeftLarmGlow
#		Type	Local
#		At	WepL
#		POther  LarmL
#		part	:KineticBodyGlowsCyan01.part
#		POther LarmL
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 40
#	End
#End
#
#
#Condition
#	On	Time
#	Time	80
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	Target
#		part	:KineticBodySwirlsCyan01.part
#		PMagnet WepL
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 40
#	End
#	
#	Event	
#		ENAME	LeftLarmGlow
#		Type	Local
#		At	WepL
#		part	:KineticBodySwirlsCyan01.part
#		PMagnet Target
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 40
#	End
#End
#
#
### BODY GLOW 1 #######################################################
#
#Condition
#	On	Time
#	Time	80
#	
#	Event	
#		ENAME	SpinNode
#		Type	Local
#		At	Hips	
#		Bhvr	:KineticScale.bhvr
#	End
#
#	Event	
#		ENAME	Swirl1
#		Type	Local
#		At	SpinNode
#		part	:KinecTestSwirl02.part
#		part	:KinecTestSwirl01.part
#		Bhvr	:KineticSpin1.bhvr
#		Lifespan 30
#	End
#
#	Event	
#		ENAME	Swirl2
#		Type	Local
#		At	SpinNode
#		part	:KinecTestSwirl02.part
#		part	:KinecTestSwirl01.part
#		Bhvr	:KineticSpin2.bhvr
#		Lifespan 30
#	End
#
#	Event	
#		ENAME	Swirl3
#		Type	Local
#		At	SpinNode
#		part	:KinecTestSwirl02.part
#		part	:KinecTestSwirl01.part
#		Bhvr	:KineticSpin3.bhvr
#		Lifespan 30
#	End
#
#	Event	
#		ENAME	Glows1
#		Type	Local
#		At	SpinNode
#		part	:KineticExpandingGlows01.part
#		part	:KineticExpandingRings01.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 30
#	End
#
#	Event	
#		ENAME	Flyaways1
#		Type	Start
#		At	Swirl1
#		part	:KineticSwirlFlyAway01.part
#		#Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 30
#	End
#
#	Event	
#		ENAME	Flyaways2
#		Type	Start
#		At	Swirl2
#		part	:KineticSwirlFlyAway02.part
#		#Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 30
#	End
#End	
#
### BODY GLOW 2 #######################################################
#
#Condition
#	On	Time
#	Time	90
#
#	Event	
#		ENAME	ChestGlow
#		Type	Local
#		At	Hips
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther Neck
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 45
#	End
#
#	Event	
#		ENAME	HeadGlow
#		Type	Local
#		At	Hair
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther Chest
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 35
#	End
#
#	Event	
#		ENAME	RightUarmGlow
#		Type	Local
#		At	SpadR
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LarmR
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 35
#	End
#	
#	Event	
#		ENAME	RightLarmGlow
#		Type	Local
#		At	WepR
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LarmR
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 30
#	End
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	SpadL
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LarmL
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 35
#	End
#	
#	Event	
#		ENAME	LeftLarmGlow
#		Type	Local
#		At	WepL
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LarmL
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 30
#	End
#
#	Event	
#		ENAME	LeftULegGlow
#		Type	Local
#		At	LlegL
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther Ulegl
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 45
#	End
#
#	Event	
#		ENAME	LeftLLegGlow
#		Type	Local
#		At	FootL
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther Llegl
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 50
#	End
#
#	Event	
#		ENAME	RightULegGlow
#		Type	Local
#		At	LlegR
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther UlegR
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 45
#	End
#
#	Event	
#		ENAME	RightLLegGlow
#		Type	Local
#		At	FootR
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LlegR
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 50
#	End
#End

#########################################################

End