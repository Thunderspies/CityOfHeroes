#########################################################
##	onfire
##
FxInfo  

Condition
	On 	Time
	Time	0

	Event
		Ename	Base
		Type	Local
		At	Origin
		
		BhvrOverride
			PyrRotate		0 0 0
		End
	End

End

Condition
	On 	Force
	ForceThreshold	100
	DoMany 1

############################################################
##Sound####
##################

	Event
		
		Type	start 
		At	Base
		#Flags OneAtATime
		
		#Sound	CashExplo 80 30 .9
		Lifespan	5
	End



############################################################
##Fx####
##################

End

Condition
	On 	Force
	ForceThreshold	50
	DoMany 1
	Time	7.5
	

	Event
		Ename	Bill
		Type	start
		At	Base
		##Flags OneAtATime

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
	
	Event
		HardwareOnly
		Ename	Billzz
		Type	start
		At	Base
		##Flags OneAtATime

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	100
	DoMany 1
	Time	10

	Event
		Ename	Bill2
		Type	start
		At	Base

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
		
			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
	
	Event
		HardwareOnly
		Ename	Billzz
		Type	start
		At	Base
		##Flags OneAtATime

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
End

Condition
	On 	Force
	ForceThreshold	250
	DoMany 1
	Time	12

	Event
		Ename	Bill3
		Type	start
		At	Base

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
	
	Event
		HardwareOnly
		Ename	Billzz
		Type	start
		At	Base
		##Flags OneAtATime

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
End

Condition
	On 	Force
	ForceThreshold	500
	DoMany 1
	Time	15

	Event
		Ename	Bill4
		Type	start
		At	Base

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
	
	Event
		HardwareOnly
		Ename	Billzz
		Type	start
		At	Base
		##Flags OneAtATime

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
End

Condition
	On 	Force
	ForceThreshold	1000
	DoMany 1
	
	Time	25

	Event
		Ename	Bill5
		Type	start
		At	Base

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
	
	Event
		HardwareOnly
		Ename	Billzz
		Type	start
		At	Base
		##Flags OneAtATime

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.3 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.3
			PyrRotateJitter		18 18 18
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		3
			PhysScale		1.5 2 1.5
		End
		Lifespan	3
	End
End

End			