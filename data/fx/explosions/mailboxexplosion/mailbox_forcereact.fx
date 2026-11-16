#########################################################
##	Force react
##

FxInfo  

Condition
	On 	Force
	ForceThreshold	120
	DoMany 1

	Event
		Ename	Dust
		Type	start 
		At	Origin
		Flags OneAtATime  
		
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part

		Lifespan	15

	End

End


Condition
	On 	Force
	ForceThreshold	250
	DoMany 1
	Time	6
	Repeat	3
	RepeatJitter	2

	Event
		Type	start 
		At	Origin  
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0
			
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.5 0.5 0.5

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02

			PhysDebris		1
			PhysDensity		3
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	2
		
		#Part	:Letter.part

	End

	Event
		HardwareOnly
		Type	start 
		At	Origin  
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0
			
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.6 0.6 0.6

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02

			PhysDebris		1
			PhysDensity		3
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	2
		
		#Part	:Letter.part

	End
End

Condition
	On 	Force
	ForceThreshold	60
	DoMany 1
	Time	3
	Repeat	1
	RepeatJitter	1

	Event
		Type	start 
		At	Origin  
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		Geom	mail_letter04
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0

			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.2 0.3 0.2

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02
			PhysDebris		1
			PhysDensity		3
			
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	2
		
		#Part	:Letter.part

	End

	Event
		HardwareOnly
		Type	start 
		At	Origin  
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		Geom	mail_letter04
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0

			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.25 0.35 0.25

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02
			PhysDebris		1
			PhysDensity		3
			
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	2
		
		#Part	:Letter.part

	End
End

Condition
	On 	Force
	ForceThreshold	500
	DoMany 1
	Time	3
	Repeat	12
	RepeatJitter	3

	Event
		Type	start 
		HardwareOnly
		At	Origin  
		
		Geom	mail_letter01
		Geom	mail_letter02
		Geom	mail_letter03
		Geom	mail_letter04
		
		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		0 1 0

			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.75 0.65 0.75

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.025

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02
			PhysDebris		1
			PhysDensity		3
			
			FadeOutLength		150
			PhysScale		1.25 2.5 1.25
		End

		CThresh 0.0001

		Lifespan	2
		
		#Part	:Letter.part

	End

End

End			