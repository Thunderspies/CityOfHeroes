#########################################################
##	
##

FxInfo

LifeSpan  430

##################################

Condition
	On 	Cycle
	Time 	3
	Chance	.88
	RepeatJitter	1

	Event
		Type	Start
		At	Origin

		BHVR	Behaviors/GenericParticleFade.Bhvr
		BhvrOverride
			StartJitter		25 2 25
			InitialVelocity		0 -.25 0
			InitialVelocityJitter	.05 .1 .05
			Gravity			0.025
			Drag			0.01

			Physics		1
			PhysGravity	.6
			PhysRadius	.5
		End
		Part	:PlasmaFire.part
		Part	:PlasmaBoltTrail.part

		CDestroy	1
		CEvent		:PlasmaBurnPatch.fx
		CThresh		0.0001

		Lifespan	30
	End
	
End

End				