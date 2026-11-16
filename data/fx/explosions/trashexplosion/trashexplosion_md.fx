#########################################################
##	onfire
##
FxInfo  

LifeSpan	800

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	origin
		
		#Sound	telekinetic3 80 30 .9

	End

End


Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	origin

		
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

	End

End

Condition
	On 	Time
	Time 	0
	
	Repeat	3
	RepeatJitter	2

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
			PositionOffset		0 .5 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.4 0
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

		Lifespan	60
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	8
	RepeatJitter	3

	Event
		Type	start 
		At	origin
		
		Geom	newpaper01
		Geom	newpaper02
		Geom	newpaper03
		
		#Geom	chips1
		
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		.25 .25 .25

			scale			.5 .5 .5
			
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.5 0.5

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

			PhysDensity		2
			PhysScale		1.2 2 1.2
			
			FadeOutLength		150
		End

		CThresh 0.0001

		Lifespan	600
		LifespanJitter	60

	End

End

Condition
	On 	Time
	Time 	0
	
	Repeat	1
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
			PositionOffset		0 .5 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.4 0
			InitialVelocityJitter	0.2 0.2 0.2
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

		Lifespan	60
	End

End

End			