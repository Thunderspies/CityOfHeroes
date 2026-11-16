#########################################################
##	onfire
##
FxInfo  

LifeSpan	600


Condition
	Time 	0

		Event
		Type	start
		At	root
				
		BHVR	Behaviors/VaultCameraShake.bhvr
				
		LifeSpan	300
	End

	Event
		EName	VaultHookup
		Type	start
		At	root
		
		BHVR	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			#PositionOffset	0 7 0
		End
		Geom	Rock_Hookup
		Sound MuDoorExplo 390 390 .97
		
		LifeSpan	300
	End


	Event
		EName	Center
		Type	start
		At	VaultHookup
		Altpiv	4
		BhvrOverride
			PositionOffset	0 2.5 0
		End
	End

###################################################
###Red electrcity archs

	Event
		EName	continuingArchs
		Type	start
		At	Chest

		BhvrOverride
			PositionOffset	0 10 0
		End
		Part1	:Red_ElectricityArch.part
		Part2	:Red_ElectricityArch2.part
		Part2	:Red_ElectricityArch2White.part
		Part2	:Red_ElectricityArchWhite.part
		LifeSpan	8
	End

End

###################################################################


Condition
	Time 	0
	Repeat	9

	Event
		EName 	SmokeSparx
		Type	start 
		At	VaultHookup
		Altpiv	4
		BhvrOverride

		PositionOffset		0 4 0
			StartJitter	10 10 5
		End

		Part	:InfernoExplosion.part
		Part	:MuDoorFlash.part
		Part	:InfernoExplosion.part
		Part	:MuDoorFlash.part
		
		LifeSpan	3

	End


End

Condition
	Time 	1
	Repeat	80

	Event
		Type	start
		At	VaultHookup
		Altpiv	4
		
		BhvrOverride
			PositionOffset		0 5 0
			StartJitter	15 20 5
		End

		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

		Lifespan	300
	End

	Event
		hardwareOnly
		Type	start
		At	VaultHookup
		Altpiv	4
		
		BhvrOverride
			PositionOffset		0 5 0
			StartJitter	15 20 5
		End

		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

		Lifespan	300
	End
End

######################################################################################################

Condition
	Time 	0
	Repeat	10

	Event
		Ename	BigMuChunks
		Type	start
		At	Center

		Geom	v_destmudoor_wall_rubble01 
		Geom	v_destmudoor_wall_rubble02  
		Geom	v_destmudoor_wall_rubble03 
		Geom	v_destmudoor_wall_rubble04 
		Geom	v_destmudoor_wall_rubble05
		Bhvr 	Behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0 5 0
			StartJitter	15 20 0
			pyrRotate	90 0 0
			Scale	1.25 1.25 1.25
			#FadeOutLength 		140

			InitialVelocity		0 0.25 0
			InitialVelocityJitter	.35 .35 .35
			SpinJitter		0.3 0.3 0.3

			Gravity			0.03 
			Physics 		1
			PhysRadius 		0.01
			PhysGravity 		1.0
			physRestitution 	0.25
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			PhysDebris		3
			PhysDensity		2
		End
		#CEvent 	:FireBallSparks.fx
		CThresh 0.001

		Lifespan	500
		LifespanJitter	60
	End

	
	Event
		HardwareOnly
		Ename	BigMuChunks
		Type	start
		At	Center

		Geom	v_destmudoor_wall_rubble01 
		Geom	v_destmudoor_wall_rubble02  
		Geom	v_destmudoor_wall_rubble03 
		Geom	v_destmudoor_wall_rubble04 
		Geom	v_destmudoor_wall_rubble05
		Bhvr 	Behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0 5 0
			StartJitter	15 20 0
			pyrRotate	90 0 0
			Scale	1.25 1.25 1.25
			#FadeOutLength 		140

			InitialVelocity		0 0.25 0
			InitialVelocityJitter	.35 .35 .35
			SpinJitter		0.3 0.3 0.3

			Gravity			0.03 
			Physics 		1
			PhysRadius 		0.01
			PhysGravity 		1.0
			physRestitution 	0.25
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			PhysDebris		3
			PhysDensity		2
		End
		#CEvent 	:FireBallSparks.fx
		CThresh 0.001

		Lifespan	500
		LifespanJitter	60
	End

End

Condition
	Time 	0
	Repeat	6

	Event
		Ename	LilMuChunks
		Type	start
		At	Center

		Geom	v_destmudoor_wall_rubble01 
		Geom	v_destmudoor_wall_rubble02  
		Geom	v_destmudoor_wall_rubble03 
		Geom	v_destmudoor_wall_rubble04 
		Geom	v_destmudoor_wall_rubble05
		Bhvr 	Behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0 5 0
			StartJitter	20 25 0
			pyrRotate	90 0 0
			Scale	.75 .75 .75
			#FadeOutLength 		140

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	.75 .75 .75
			SpinJitter		0.3 0.3 0.3

			Gravity			0.03 
			Physics 		1
			PhysRadius 		0.01
			PhysGravity 		1.0
			physRestitution 	0.25
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			PhysDebris		3
			PhysDensity		2
		End
		#CEvent 	:FireBallSparks.fx
		CThresh 0.001

		Lifespan	500
		LifespanJitter	60
	End

	Event
		HardwareOnly
		Ename	LilMuChunks
		Type	start
		At	Center

		Geom	v_destmudoor_wall_rubble01 
		Geom	v_destmudoor_wall_rubble02  
		Geom	v_destmudoor_wall_rubble03 
		Geom	v_destmudoor_wall_rubble04 
		Geom	v_destmudoor_wall_rubble05
		Bhvr 	Behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0 5 0
			StartJitter	20 25 0
			pyrRotate	90 0 0
			Scale	.75 .75 .75
			#FadeOutLength 		140

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	.75 .75 .75
			SpinJitter		0.3 0.3 0.3

			Gravity			0.03 
			Physics 		1
			PhysRadius 		0.01
			PhysGravity 		1.0
			physRestitution 	0.25
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			PhysDebris		3
			PhysDensity		2
		End
		#CEvent 	:FireBallSparks.fx
		CThresh 0.001

		Lifespan	500
		LifespanJitter	60
	End
End


Condition
	Time 	0
	Repeat	5

	Event
		Ename	Rocky
		Type	start
		At	VaultHookup
		Altpiv	4

		Geom	Cav_rockSM_01  
		Geom	Cav_rockSM_02  
		Geom	Cav_rockSM_03  
		Geom	Cav_rockSM_04  
		Bhvr 	:VaultPhys.bhvr
		BhvrOverride
			PositionOffset	0 2.5 0
			StartJitter	10 10 0
			InitialVelocityJitter	.8 .8 .8
		End
		#CEvent 	:FireBallSparks.fx
		CThresh 0.1

		Lifespan	300
		LifespanJitter	60
	End

	Event
		HardwareOnly
		Ename	LilMuChunks
		Type	start
		At	Center

		Geom	v_destmudoor_wall_rubble01 
		Geom	v_destmudoor_wall_rubble02  
		Geom	v_destmudoor_wall_rubble03 
		Geom	v_destmudoor_wall_rubble04 
		Geom	v_destmudoor_wall_rubble05
		Bhvr 	Behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0 5 0
			StartJitter	20 25 0
			pyrRotate	90 0 0
			Scale	.75 .75 .75
			#FadeOutLength 		140

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	.75 .75 .75
			SpinJitter		0.3 0.3 0.3

			Gravity			0.03 
			Physics 		1
			PhysRadius 		0.01
			PhysGravity 		1.0
			physRestitution 	0.25
			physSFriction 	 	0.9
			physDFriction 	 	0.8
			PhysDebris		3
			PhysDensity		2
		End
		#CEvent 	:FireBallSparks.fx
		CThresh 0.001

		Lifespan	500
		LifespanJitter	60
	End
End


End			
