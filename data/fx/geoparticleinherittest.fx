#########################################################
## Test System
#########################################################

FxInfo  

Lifespan 180

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	base
		Type	Create
		Inherit	Position SuperRotation 
		Update	Position
		At	Root
	End
End

##################### Tires  ######################

Condition
	On 	Time
	Time 	0

	Event
		Ename	TireRightRear1
		Type	Local
		At	base
		Geom	_PPD_wheelLarge
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			StartColor		255 255 255
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			InitialVelocity		1.0 1.0 1.0
			InitialVelocityJitter	0.0 0.0 0.0
			physRestitution 	0.7
			PhysDebris		3
		End
		Lifespan	30
	End
End

#######################################################################

End