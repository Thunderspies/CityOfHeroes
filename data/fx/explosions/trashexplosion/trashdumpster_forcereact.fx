#########################################################
##	onfire
##
FxInfo  


OnForceRadius 4

#LifeSpan	800


Condition
	On 	Force
	ForceThreshold	150
	DoMany 1
	Time		30

	Repeat	1

	Event
		Type	start 
		At	Origin

		BhvrOverride
			StartJitter	1 1 1
		End
			
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

	End

End

Condition
	On 	Force
	ForceThreshold	75
	DoMany 1
	Time		10
	
	Repeat	1
	RepeatJitter	1

	Event
		Type	start
		At	Origin

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
			PositionOffset		0 1 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.75 -.1
			InitialVelocityJitter	0.5 0.5 0.5
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
			FadeOutLength		50
			
			PhysDebris		1
			PhysDensity		2
		End
		
		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	125
	DoMany 1
	Time		9
	
	Repeat	3
	RepeatJitter	1

	Event
		HardwareOnly
		Type	start
		At	Origin

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
			PositionOffset		0 1 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.75 -.1
			InitialVelocityJitter	0.5 0.5 0.5
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
			FadeOutLength		50
			
			PhysDebris		1
			PhysDensity		2
		End
		
		Lifespan	3
	End

End
Condition
	On 	Force
	ForceThreshold	45
	DoMany 1
	Time		12
	Chance		.75

	RepeatJitter	1

	Event
		Type	start 
		At	Origin
		
		Geom	newpaper01
		Geom	newpaper02
		Geom	newpaper03
		
		#Geom	chips1
		
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		.25 .25 .25

			scale			.4 .4 .4
			
			InitialVelocity		.0 1.25 -.25
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

			
			FadeOutLength		50
		End

		Lifespan	6

	End

End

Condition
	On 	Force
	ForceThreshold	200
	DoMany 1
	Time		20
	
	RepeatJitter	1

	Event
		Type	start
		At	Origin

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.65 -.15
			InitialVelocityJitter	0.4 0.4 0.4
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
			FadeOutLength		50
			
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
	Time		16
	
	Repeat		3
	RepeatJitter	1

	Event
		HardwareOnly
		Type	start
		At	Origin

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.65 -.15
			InitialVelocityJitter	0.4 0.4 0.4
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
			FadeOutLength		50
			
			PhysDebris		1
			PhysDensity		1.5
		End

		Lifespan	3
	End

End


Condition
	On 	Force
	ForceThreshold	350
	DoMany 1
	Time		30

	Chance	.75

	Event
		Type	start
		At	Origin

		geom	trashbag_black
		geom	trashbag_white
	
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1.5 0		
			scale			.8 .8 .8

			InitialVelocity		0 0.5 -.25
			InitialVelocityJitter	0.5 0.25 0.35
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.2
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			FadeOutLength		50
			PhysDebris		3
		End

		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	450
	DoMany 1
	Time		15
	
	Repeat	1
	RepeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	bottle1
		Geom	bottle2
		
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44

		
		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.75 -.5
			InitialVelocityJitter	0.75 0.5 0.75
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
			FadeOutLength		50
			
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
	Time		12
	
	Repeat	3
	RepeatJitter	1

	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	bottle1
		Geom	bottle2
		
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44

		
		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.75 -.5
			InitialVelocityJitter	0.75 0.5 0.75
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
			FadeOutLength		50
			
			PhysDebris		1
			PhysDensity		2
		End
		
		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	600
	DoMany 1
	Time		40

	Chance		.5
	

	Event
		Type	start
		At	Origin

		geom	trashbag_black
		geom	trashbag_white
	
		#Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 2 0		
			scale			.8 .8 .8

			InitialVelocity		0 0.6 -.5
			InitialVelocityJitter	0.75 0.35 0.35
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.015

			physics 		1
			physRadius 		0.8
			physGravity 		.8
			Gravity			.04
			physRestitution 	0.2
			physSFriction 	 	0.9
			physDFriction 	 	0.8
		End

		CEvent	:TrashExplosion_MD.Fx
		CDestroy	1
		CThresh	.0001

		Lifespan	600
		LifespanJitter	30
	End

End


End			