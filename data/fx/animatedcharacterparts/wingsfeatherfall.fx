
############################################################################
FxInfo  

##########################################################################
##Landing Fx
########################################

Condition
	On	TriggerBits
#	TriggerBits	Land
	TriggerBits	concrete
	TriggerBits	grass		#needs grass clipping fx#
	TriggerBits	gravel		#needs gravel debris fx#
	TriggerBits	Dirt
	TriggerBits	Wood
	TriggerBits	Carpet
	TriggerBits	metal

	DoMany		1
		
	Event
		EName 	Prime
		Type	Local 
		At	Back
		Until	Air
		Flags	OneAtaTime
		
		ChildFx	:FeatherBurst.fx
		#powers/Water/WirlPool.fx
		

		LifeSpan	200
		
	End

End

Condition
	On 	Force
	ForceThreshold	29
	DoMany	1
	Time	170


	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.9 .9 .9
			SpinJitter		.2 .3 .1
			StartJitter		.5 .5 .5
			InitialVelocity		.0 0.05 .0
			InitialVelocityJitter	.8 .1 .8
			PyrRotateJitter		180 180 180
			Drag			0.25
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	120
	DoMany 1
	Time		16

	Repeat	2
	repeatJitter	2
	

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.9 .9 .9
			SpinJitter		.2 .3 .1
#			StartJitter		12 20 12
			InitialVelocity		.0 0.05 .0
			InitialVelocityJitter	.8 .1 .8
			PyrRotateJitter		180 180 180
			Drag			0.25
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	150
	DoMany 1
	Time		12

	repeatJitter	2

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather
		
		BhvrOverride
			scale			.9 .9 .9
			SpinJitter		.1 .1 .1
#			StartJitter		10 15 10
			InitialVelocity		.0 0.05 .0
			InitialVelocityJitter	.8 .1 .8
			PyrRotateJitter		180 180 180
			Drag			0.005
			physics 		1
			physRadius 		1.8
			physGravity 		1
			Gravity			0.04
			physRestitution 	0.1
			physSFriction 	 	0.95
			physDFriction 	 	0.8
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End

Condition
	On 	Force
	ForceThreshold	250
	DoMany 1
	Time		30
	
	Time		3
	
	Repeat	2
	repeatJitter	2

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.9 .9 .9
			SpinJitter		.2 .3 .1
#			StartJitter		12 20 12
			InitialVelocity		.0 0.05 .0
			InitialVelocityJitter	.8 .1 .8
			PyrRotateJitter		180 180 180
			Drag			0.25
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

	Event
		HardwareOnly
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.9 .9 .9
			SpinJitter		.2 .3 .1
#			StartJitter		12 20 12
			InitialVelocity		.0 0.05 .0
			InitialVelocityJitter	.8 .1 .8
			PyrRotateJitter		180 180 180
			Drag			0.25
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End


Condition
	On 	Force
	ForceThreshold	400
	DoMany 1
	
	Time		20
	
	Repeat	3
	repeatJitter	3

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather
		
		BhvrOverride
			scale			.9 .9 .9
			SpinJitter		.1 .1 .1
#			StartJitter		10 15 10
			InitialVelocity		.0 0.05 .0
			InitialVelocityJitter	.8 .1 .8
			PyrRotateJitter		180 180 180
			Drag			0.005
			physics 		1
			physRadius 		1.8
			physGravity 		1
			Gravity			0.04
			physRestitution 	0.1
			physSFriction 	 	0.95
			physDFriction 	 	0.8
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End


Condition
	On 	Force
	ForceThreshold	500
	DoMany 1
	
	Time		10
	
	Repeat	8
	repeatJitter	4

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		BhvrOverride
			scale			.9 .9 .9
			SpinJitter		.2 .3 .1
#			StartJitter		12 20 12
			InitialVelocity		.0 0.05 .0
			InitialVelocityJitter	.8 .1 .8
			PyrRotateJitter		180 180 180
			Drag			0.25
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

End			