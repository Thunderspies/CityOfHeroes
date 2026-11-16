#########################################################
##	
##

FxInfo

LifeSpan  250

##################################


Condition
	On 	Time
	Time 	0

	Event
		Ename	Base
		Type	Start
		At	T_Root

		BHVR	Behaviors/GenericParticleFade.Bhvr
		BhvrOverride
			PositionOffset	0 15 0
		End
		
	End

	Event
		Type	Start
		At	Base

		BhvrOverride
			InitialVelocity		0 -.1 0
			Drag			0.01
		End
		
		ChildFX	:PlasmaRain.fx

		Lifespan	80
	End
	
End

Condition
	On 	Time
	Time 	30

	Event
		Ename	Prime
		Type	Start
		At	Base
		BhvrOverride
			PositionOffset	0 15 0
			TrackRate	2
		End
		Magnet	Base

		lifespan	60

	End

End

Condition
	On 	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
		
	End

End

End				