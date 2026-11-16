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
		At	chest
		
		#Sound	trashcanexplo1 80 30 .9

	End

End


Condition
	On 	Time
	Time 	0

	Event
		Type	start 
		At	chest
	
		
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	Can
		Type	start
		At	chest
		Geom	exp_garbCan_empty01
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		0 0 0		
			scale			1 1 1

			InitialVelocity		0 0.35 0
			InitialVelocityJitter	0.25 0.15 0.25

			StartColor		200 200 200
			PyrRotateJitter		0 0 0

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.7
			physDFriction 	 	0.5
			FadeOutLength		510

			PhysDensity		1.2
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	600
		lifespanJitter	120
	End

End

Condition
	On 	Time
	Time 	0
	
	Repeat	7
	RepeatJitter	3

	Event
		Type	start
		At	chest

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
			PositionOffset		0 1.5 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.2 0
			InitialVelocityJitter	0.25 0.25 0.25
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

	Event
		HardwareOnly
		Type	start
		At	chest

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		
		Geom	GEO_Slag_Golem_Junk_13

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1.5 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.2 0
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

		Lifespan	20
		LifespanJitter	10
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	12
	RepeatJitter	5

	Event
		Type	start 
		At	chest
		
		Geom	newpaper01
		Geom	newpaper02
		Geom	newpaper03
		
		#Geom	chips1
		
		BhvrOverride
			PositionOffset		0 1.5 0
			StartJitter		.25 .25 .25

			scale			.5 .5 .5
			
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.55 0.25 0.55

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
		LifespanJitter	90

	End

	Event
		HardwareOnly
		Type	start
		At	chest

		Geom	bottle1
		Geom	bottle2
		
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		
		Geom	GEO_Slag_Golem_Junk_25
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1.5 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.2 0
			InitialVelocityJitter	0.25 0.25 0.25
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
		
		Lifespan	20
		LifespanJitter	10

		End
End

Condition
	On 	Time
	Time 	0
	
	Repeat	2
	RepeatJitter	1

	Event
		Type	start
		At	chest

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1.5 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.2 0
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

Condition
	On 	Time
	Time 	0
	
	Chance	.4
	RepeatJitter	1

	Event
		Type	start
		At	chest

		geom	trashbag_black
	
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 1.5 0
			StartJitter		.25 .25 .25		
			scale			.6 .6 .6

			InitialVelocity		0 0.2 0
			InitialVelocityJitter	0.2 0.15 0.2
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
			FadeOutLength		510
			
			PhysDebris		3
		End

		Lifespan	60
	End

End

End			