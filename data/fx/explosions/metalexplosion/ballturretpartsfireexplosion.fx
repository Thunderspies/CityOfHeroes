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

######################################################################
##Curled metal Pieces
#############################################

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		
		Bhvr	Behaviors\BallTurretMetalOffset.bhvr
		BhvrOverride

			scale			.015 .015 .015
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		Bhvr	Behaviors\BallTurretMetalOffset2.bhvr
		BhvrOverride


			scale			.015 .015 .015
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		Bhvr	Behaviors\BallTurretMetalOffset3.bhvr
		BhvrOverride

			scale			.015 .015 .015
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		Bhvr	Behaviors\BallTurretMetalOffset4.bhvr
		BhvrOverride

			scale			.015 .015 .015
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		Bhvr	Behaviors\BallTurretMetalOffset5.bhvr
		BhvrOverride

			scale			.015 .015 .015
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Repeat	6

	Event
		Type	start
		At	Chest

		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		
		Bhvr	V_COV\PhysicsEnabled\BallTurretMetalChunkScatter.bhvr
		BhvrOverride

			scale			.015 .015 .015
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

#####################################################################################

Condition
	On 	Time
	Time 	0
	Repeat	3

	Event
		Type	start
		At	chest
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum

		Bhvr	Behaviors\BallTurretMetalOffset.bhvr

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

			PhysDebris		3
			PhysDensity		2

			scale			.2 .2 .2		
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	5

	Event
		HardwareOnly
		Type	start
		At	chest
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum
		
		Bhvr	Behaviors\BallTurretMetalOffset.bhvr
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

			PhysDebris		3
			PhysDensity		2
			
			scale			.2 .2 .2
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
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
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum
		
		Bhvr	Behaviors\BallTurretMetalOffset.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			scale			.2 .2 .2

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
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum

		Bhvr	Behaviors\BallTurretMetalOffset2.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			scale			.2 .2 .2

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
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum

		Bhvr	Behaviors\BallTurretMetalOffset3.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			scale			.2 .2 .2
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
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum

		Bhvr	Behaviors\BallTurretMetalOffset4.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			scale			.2 .2 .2
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
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum

		Bhvr	Behaviors\BallTurretMetalOffset5.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			
			scale			.2 .2 .2

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
		Type	start
		At	Chest
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum
		
		Bhvr	V_COV\PhysicsEnabled\BallTurretMetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			scale			.2 .2 .2
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
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		#Geom	Widget_25_Alluminum
		#Geom	Widget_24_Alluminum
		Geom	Widget_26_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_23_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_19_Alluminum		
		Geom	Widget_15_Alluminum
		Geom	Widget_18_Alluminum
		
		Bhvr	V_COV\PhysicsEnabled\BallTurretMetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			scale			.2 .2 .2
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
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