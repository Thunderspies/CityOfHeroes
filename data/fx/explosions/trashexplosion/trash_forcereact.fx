#########################################################
##	onfire
##
FxInfo  


OnForceRadius 2

#LifeSpan	800

Condition
	On 	Force
	ForceThreshold	200
	DoMany 1
	Time		30

	Event
		Type	start 
		At	origin
			
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

	End

End

Condition
	On 	Force
	ForceThreshold	60
	DoMany 1
	Time		5
	
	RepeatJitter	1

	Event
		Type	start
		At	origin

		Geom	bottle1
		Geom	bottle2
		
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		.15 .15 .15		
			scale			1 1 1

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.35 0.35 0.35
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		.75
			physRestitution 	0.5
			physSFriction 	 	0.7
			physDFriction 	 	0.6
			FadeOutLength		510
			
			PhysDebris		1
			PhysDensity		2
		End
		
		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	100
	DoMany 1
	Time		5
	
	Repeat	2

	Event
		HardwareOnly
		Type	start
		At	origin

		Geom	bottle1
		Geom	bottle2
		
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		.15 .15 .15		
			scale			1 1 1

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.35 0.35 0.35
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		.75
			physRestitution 	0.5
			physSFriction 	 	0.7
			physDFriction 	 	0.6
			FadeOutLength		510
			
			PhysDebris		1
			PhysDensity		2
		End
		
		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	35
	DoMany 1
	Time		10
	Chance		.65

	RepeatJitter	1

	Event
		Type	start 
		At	origin
		
		Geom	newpaper01
		Geom	newpaper02
		Geom	newpaper03
		
		#Geom	chips1
		
		BhvrOverride
			StartJitter		.15 .15 .15

			scale			.3 .3 .3
			
			InitialVelocity		.0 1 .0
			InitialVelocityJitter	0.75 0.5 0.75

			PyrRotateJitter		180 180 180
			SpinJitter		0.2 0.2 0.2

			Drag			0.065

			physics 		1
			physRadius 		3
			physGravity 		.5
			physRestitution 	0.15
			physSFriction 	 	0.7
			physDFriction 	 	0.7
			Gravity 		.02

			PhysDebris		1
			PhysDensity		2
			PhysScale		1.2 2 1.2

			
			FadeOutLength		150
		End

		Lifespan	6

	End

End

Condition
	On 	Force
	ForceThreshold	175
	DoMany 1
	Time		15
	
	RepeatJitter	1

	Event
		Type	start
		At	origin

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		.05 .05 .05		
			scale			1 1 1

			InitialVelocity		0 0.65 0
			InitialVelocityJitter	0.25 0.25 0.25
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.3
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			FadeOutLength		510
			
			PhysDebris		1
			PhysDensity		1.5
		End

		Lifespan	3
	End

End


Condition
	On 	Force
	ForceThreshold	250
	DoMany 1
	Time		15
	
	Repeat		3

	Event
		HardwareOnly
		Type	start
		At	origin

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		.05 .05 .05		
			scale			1 1 1

			InitialVelocity		0 0.65 0
			InitialVelocityJitter	0.25 0.25 0.25
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.3
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			FadeOutLength		510
			
			PhysDebris		1
			PhysDensity		1.5
		End

		Lifespan	3
	End

End

End			