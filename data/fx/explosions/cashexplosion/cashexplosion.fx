#########################################################
##	onfire
##
FxInfo  

LifeSpan	600

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		Sound CashExplo 80 80 .9

	End

End


Condition
	Time 	0
	
	Repeat	15
	RepeatJitter	5

	Event
		Type	start
		At	chest

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.5 0.5
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	300
		LifespanJitter	60
	End

	Event
		HardwareOnly
		Type	start
		At	chest

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.5 0.5
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	300
		LifespanJitter	60
	End
End

Condition
	Time 	0
	
	Repeat	15
	RepeatJitter	5

	Event
		Type	start
		At	Chest

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			scale			1 1 1
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.2 0.3 0.2
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	300
		LifespanJitter	60
	End

	Event
		HardwareOnly
		Type	start
		At	Chest

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			scale			1 1 1
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.2 0.3 0.2
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	300
		LifespanJitter	60
	End
End


End			