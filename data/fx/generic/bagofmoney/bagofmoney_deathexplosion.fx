#####################
## Bag of Money Explosion FX ##
#####################

FxInfo

LifeSpan	0

Condition
	On		Time
	
	Event
		EName 	Prime1
		Type	Local 
		At	back
		Lifespan	1

		Sound CashExplo 80 80 .9

	End

End

Condition
	On		Time
	DoMany		1

	Repeat		64
	RepeatJitter	32

	Event   
		Type	start
		At	back

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		0.2 0.1 0.1
			InitialVelocity		0.25 0.25 0.25
			InitialVelocityJitter	0.125 0.25 0.125
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		0.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End

		CThresh 0.0001
		Lifespan	300
		LifespanJitter	60
	End

End


End