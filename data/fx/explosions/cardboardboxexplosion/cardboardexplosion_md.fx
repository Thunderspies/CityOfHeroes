#########################################################
##	onfire
##
FxInfo  

LifeSpan	1200

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Sound cardboardexplo3 80 80 .9

	End

End


Condition
	On 	Time
	Time 	0
	Repeat	1

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
	
	Repeat	5
	RepeatJitter	2

	Event
		SoftwareOnly
		Type	start
		At	chest
		Geom	exp_cardboard_panel01
		Geom	exp_cardboard_panel02
		Geom	exp_cardboard_panel02
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		.25 .25 .25		
			scale			.7 .7 .7

			InitialVelocity		0 0.4 0
			InitialVelocityJitter	0.5 0.35 0.5
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		.75
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		510
			PhysScale		1.2 1.2 3
			PhysDensity		2
		End

		Lifespan	600
		LifespanJitter	60
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
		
#		Geom	paper1
#		Geom	paper2
#		Geom	paper3
		
		Geom	newpaper01
		Geom	newpaper02
		Geom	newpaper03
		
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		.5 .5 .5

			scale			.5 .5 .5
			
			InitialVelocity		.0 0.65 .0
			InitialVelocityJitter	0.65 0.5 0.65

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

			
			FadeOutLength		150
		End

		CThresh 0.0001

		Lifespan	300
		LifespanJitter	90
		
		#Part	:Letter.part

	End

End

############################################################
##Hardware Addition   =more parts come out of boxes=

Condition
	Time 	0
	Repeat	50

	Event
		HardwareOnly

		Type	start 
		At	Chest
		
		Bhvroverride
			
#			Startcolor	255 180 180

			PositionOffset		0 0 0
			InitialVelocity		0.0 0.25 0.0		
			InitialVelocityJitter	0.4 0.3 0.4
			pyrRotateJitter		180 180 180
			SpinJitter		.5 1.5 .5
			Drag			0.012
			physics 		1
			physRadius 		.001
			physGravity		.5
			Gravity			.02
			physRestitution 	0.9
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			FadeOutLength		200
			
			PhysDebris		1
			PhysDensity		2

			Scale			1 1 1
			PhysScale		2 2 2
		End
		
		Lifespan	300
		LifespanJitter	60
		Geom	Foam_peanut
		#Part	:QuarterHeads.part
	End

End


Condition
	On 	Time
	Time 	0
	
	Repeat	5
	RepeatJitter	2

	Event
		HardwareOnly
		Type	start
		At	chest
		Geom	exp_cardboard_panel01
		Geom	exp_cardboard_panel02
		Geom	exp_cardboard_panel02
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		.25 .25 .25		
			scale			.7 .7 .7

			InitialVelocity		0 0.4 0
			InitialVelocityJitter	0.5 0.35 0.5
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.025

			physics 		1
			physRadius 		0.8
			physGravity 		.75
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		510
			PhysScale		1.2 1.2 3
			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	600
		LifespanJitter	60
	End

End

Condition
	Random	1

	Event
		Ename	Electronics
		HardwareOnly
		Type	start
		At	chest
	End

	Event
		Ename	Hardware
		HardwareOnly
		Type	start
		At	chest
	End
End

Condition
	Time 	0
	Repeat	6

	Event
		HardwareOnly
		Type	start
		At	Hardware
#		Geom	Widget_25_Alluminum
#		Geom	Widget_24_Alluminum
#		Geom	Widget_26_Alluminum
#		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
#		Geom	Widget_17_Alluminum
#		Geom	Widget_23_Alluminum
#		Geom	Widget_21_Alluminum
#		Geom	Widget_16_Alluminum
#		Geom	Widget_19_Alluminum		
#		Geom	Widget_15_Alluminum
#		Geom	Widget_18_Alluminum

		BhvrOverride


Spin			0.1 0.1 0.1
SpinJitter		0.5 0.5 0.5

Gravity			0.04 

			InitialVelocity		0 .5 0
			InitialVelocityJitter	.2 .2 .2
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.5
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110

			PhysDebris		1
			PhysDensity		3

			Scale			.15 .15 .15		
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Repeat	8

	Event
		HardwareOnly
		Type	start
		At	Hardware
#		Geom	Widget_25_Alluminum
#		Geom	Widget_24_Alluminum
#		Geom	Widget_26_Alluminum
#		Geom	Widget_22_Alluminum
#		Geom	Widget_20_Alluminum
#		Geom	Widget_17_Alluminum

#		Geom	Widget_23_Alluminum
#		Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
#		Geom	Widget_19_Alluminum		
#		Geom	Widget_15_Alluminum
#		Geom	Widget_18_Alluminum

		BhvrOverride


Spin			0.1 0.1 0.1
SpinJitter		0.5 0.5 0.5

Gravity			0.04 

			InitialVelocity		0 .5 0
			InitialVelocityJitter	.2 .2 .2
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.5
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110

			PhysDebris		1
			PhysDensity		3

			Scale			.13 .13 .13		
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Repeat	10

	Event
		HardwareOnly
		Type	start
		At	Hardware
		Geom	Widget_25_Alluminum
#		Geom	Widget_24_Alluminum
#		Geom	Widget_26_Alluminum
#		Geom	Widget_22_Alluminum
#		Geom	Widget_20_Alluminum
#		Geom	Widget_17_Alluminum
#		Geom	Widget_23_Alluminum
#		Geom	Widget_21_Alluminum
#		Geom	Widget_16_Alluminum
#		Geom	Widget_19_Alluminum		
#		Geom	Widget_15_Alluminum
#		Geom	Widget_18_Alluminum

		BhvrOverride


Spin			0.1 0.1 0.1
SpinJitter		0.5 0.5 0.5

Gravity			0.04 

			InitialVelocity		0 .5 0
			InitialVelocityJitter	.2 .2 .2
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.1
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110

			PhysDebris		1
			PhysDensity		3

			Scale			.1 .1 .1		
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
#	Random	1
	Time 	0
#	Repeat	15

	Event
		HardwareOnly
		Type	start
		At	Electronics
		
		Geom	Computer_Monitor_01
		Geom	Computer_PC_01
		Geom	TV_Generic_Med
		Geom	TV_Generic_Sm	

		BhvrOverride


Spin			0.1 0.1 0.1
SpinJitter		0.5 0.5 0.5

Gravity			0.04 

			InitialVelocity		0 .5 0
			InitialVelocityJitter	.2 .2 .2
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.5
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110

			PhysDebris		3
			PhysDensity		2

#			Scale			.1 .1 .1		
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End


End			