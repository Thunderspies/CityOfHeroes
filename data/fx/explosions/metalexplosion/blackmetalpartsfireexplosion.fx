#########################################################
##	onfire
##
FxInfo  

LifeSpan	1100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		Sound metal_explo2 110 110 .9

	End

End

Condition

	Event
		Type	start 
		At	chest
#		Bhvr	Behaviors/FadeOut.bhvr
		ChildFx	Explosions\fireExplosion\FireExplosionMedium.fx

	End

End


Condition

	Event
		Type	start 
		At	chest
#		Bhvr	Behaviors/FadeOut.bhvr
		ChildFx	Explosions\MetalExplosion\BlackMetalExplosion.fx

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start
		At	chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14

		Bhvr	Behaviors\BlackMetalOffset.bhvr

		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110

			PhysDebris		1
			PhysDensity		2

			Scale			.35 .35 .35		
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start
		At	chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14
		
		Bhvr	Behaviors\BlackMetalOffset.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110

			PhysDebris		1
			PhysDensity		2
			
			Scale			.35 .35 .35
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14
		
		Bhvr	Behaviors\BlackMetalOffset.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			Scale			.35 .35 .35

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14

		Bhvr	Behaviors\BlackMetalOffset2.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			Scale			.35 .35 .35

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14

		Bhvr	Behaviors\BlackMetalOffset3.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			Scale			.35 .35 .35
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14

		Bhvr	Behaviors\BlackMetalOffset4.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			Scale			.35 .35 .35
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14

		Bhvr	Behaviors\BlackMetalOffset5.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			Scale			.35 .35 .35

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	0
	Repeat	6

	Event
		SoftwareOnly
		Type	start
		At	Chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14
		
		Bhvr	V_COV\PhysicsEnabled\BlackMetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			Scale			.35 .35 .35
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

	Event
		HardwareOnly
		Type	start
		At	Chest
#		Geom	Metal_Curl
#		Geom	Metal_Flat
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10		
		Geom	Widget_11
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14
		
		Bhvr	V_COV\PhysicsEnabled\BlackMetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			Scale			.35 .35 .35

			PhysDebris		3
			PhysDensity		2
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	120

	Event
		EName 	All
		Type	Destroy 
	End


End


End			