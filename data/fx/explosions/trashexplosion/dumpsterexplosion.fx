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
		
		Sound dumpsterexplo2 80 80 .9

	End


	Event
		Ename	Base
		Type	Local 
		At	Root
	
	End
End


Condition
	On 	Time
	Time 	0
	Repeat	7

	Event
		Type	start 
		At	Base
	
		BhvrOverride
			PositionOffset	0 3 0
			StartJitter	5 4 5
		End

		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

	End

End

############3   Dumpster   ################################

Condition
	On 	Time
	Time 	0

	RepeatJitter	1

	Event
		Ename	bars
		Type	start
		At	Base
		Geom	exp_dumpster_bar
		
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 6 0
			StartJitter		4 0 4		
			scale			.8 .8 .8

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	0.25 0.25 0.25

			StartColor		140 130 120
			PyrRotateJitter		0 0 0
			SpinJitter		.3 .2 .1

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510

			PhysDebris	1
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	1

	Event
		Ename	lid1
		Type	start
		At	Base
		Geom	exp_dumpster_lid

		
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		3 7 0		
			StartJitter		2 0 2	
			scale			1 1 1

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25

			StartColor		140 130 120
			PyrRotateJitter		0 0 0
			SpinJitter		.1 .2 .1

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510

			PhysDebris		1
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	1

	Event
		Ename	lid2
		Type	start
		At	Base

		Geom	exp_dumpster_lid

		
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-3 7 0	
			StartJitter		2 0 2		
			scale			1 1 1

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.25 0.25

			StartColor		140 130 120
			PyrRotateJitter		0 0 0
			SpinJitter		.1 .2 .1

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510

			Physdebris	1
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	panelL
		Type	start
		At	Base
	
		Geom	exp_dumpster_panel02
		
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		5 3 0		
			scale			1 1 1

			InitialVelocity		.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			StartColor		140 130 120
			PyrRotateJitter		0 90 0
			
			Spin			0 0 -.15
			SpinJitter		.2 .1 .1

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			Physdebris	1
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	panelR
		Type	start
		At	Base
	
		Geom	exp_dumpster_panel02
		
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-5 3 0
			scale			1 1 1

			InitialVelocity		-.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			StartColor		140 130 120
			PyrRotateJitter		0 -90 0
			
			Spin			0 0 .15
			SpinJitter		.2 .1 .1

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			Physdebris	1
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	1

	Event
		Ename	panelF
		Type	start
		At	Base
	
		Geom	exp_dumpster_panel01
		Geom	exp_dumpster_panel01
		Geom	exp_dumpster_panel03
		
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 4	
			StartJitter		6 0 0
			scale			1 1 1

			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25

			StartColor		140 130 120
			PyrRotateJitter		0 0 0

			Spin			.15 0 0
			SpinJitter		.1 .1 .2

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			Physdebris	1
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	1

	Event
		Ename	panelB
		Type	start
		At	Base
	
		Geom	exp_dumpster_panel01
		Geom	exp_dumpster_panel01
		Geom	exp_dumpster_panel03
		
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 -4		
			StartJitter		6 0 0	
			scale			1 1 1

			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25

			StartColor		140 130 120
			PyrRotateJitter		0 90 0
			
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			Physdebris	1
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	4

	Event
		Ename	chunks
		Type	start
		At	Base

		Geom	exp_dumpster_wheel
		Geom	exp_dumpster_wheel
		Geom	exp_dumpster_wheel
		Geom	exp_dumpster_chunk
		Geom	exp_dumpster_panel03
		Geom	exp_dumpster_panel03
		
		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 0
			StartJitter		8 1.5 8		
			scale			.8 .8 .8

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	0.5 0.5 0.5

			StartColor		140 130 120
			PyrRotateJitter		0 180 0
			SpinJitter		.2 .2 .2

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
			Physdebris	1
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	On	Time
	Time 	3
	Repeat	12

	Event
		Ename	parts3
		HardwareOnly
		Type	Start
		At	base
		
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		Geom	Widget_15_Alluminum
		#Geom	Widget_16_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		#Geom	Widget_19_Alluminum
		#Geom	Widget_20_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_22_Alluminum
		#Geom	Widget_23_Alluminum
		Geom	Widget_24_Alluminum
		#Geom	Widget_25_Alluminum
		Geom	Widget_26_Alluminum

		Bhvr	:TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 0
			StartJitter		3 2 3
			
			StartColor		120 120 130

			PyrRotateJitter		180 180 180

			Scale			.5 .5 .5
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	.75 0.75 .75

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		1
			PhysDensity		2
			
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	20
		LifespanJitter	15
	End
End

End			