#########################################################
##	
##

FxInfo

LifeSpan  250

##################################


Condition
	On 	Time
	Time 	5

	Event

		Type	Start
		At	Origin

		BHVR	Behaviors/GenericParticleFade.Bhvr
		
		
		Part	:PlasmaCascadeFire.part

		LifeSpan	40

	End
	
End

Condition
	On 	Time
	Time 	5
	Repeat	10

	Event
		Type	Start
		At	Origin

		BHVR	Behaviors/GenericParticleFade.Bhvr
		BhvrOverride
			InitialVelocity		0 .1 0
			InitialVelocityJitter	.6 .4 .6
			Gravity			0.015
			Drag			0.01
		End
		
		Part	:PlasmaFire.part

		Lifespan	30
		LifeSpanJitter	15
	End
	
End

Condition
	On 	Time
	Time 	5
	Repeat	15

	Event
		Type	Start
		At	Origin

		BHVR	Behaviors/GenericParticleFade.Bhvr
		BhvrOverride
			InitialVelocity		0 .05 0
			InitialVelocityJitter	.8 .1 .8
			Gravity			0.0025
			Drag			0.01
		End
		
		Part	:PlasmaFire.part

		Lifespan	30
		LifeSpanJitter	15
	End
	
End

##################################

#Condition
#	On 	Time
#	Time 	10
#
#	Event
#		Type	Start
#		At	Origin
#
#		ChildFX	:PlasmaRainPet.fx
#
#		Lifespan	80
#	End
#	
#End

End				