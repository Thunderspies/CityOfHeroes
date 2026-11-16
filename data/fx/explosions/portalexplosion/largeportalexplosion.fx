#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	1300

##########################################################################################################
##Final Blast
######################################################################################3

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound portalexplo2 560 560 .95
	End


End

Condition
	On 	Time
	Time 	6

	Event
		EName 	Primex
		Type	Local 
		At	Root
		BhvrOverride
			PositionOffset	0 6 0
		End		
		Part	:metalBits.part
		Part	:metalBits2.part
		LifeSpan	120
	End
		
	Event
		Ename	ExplosiveForce
		Type	Start
		At	origin
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		60
			PhysForcePower		1000
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End

End

Condition
	On 	Time
	Time 	6

	
	Event
		Type	Local 
		At	root
	
		BhvrOverride
			PositionOffset	0 6 0
		End		
#		Part	World/Events/InfernoExplosionGlow.part
		Part	:LightStreaks.part
	
		LifeSpan	7

	End

End


Condition
	On 	Time
	Time 	8

	Event
		EName 	Prime
		Type	Local 
		At	hips
		BhvrOverride
		
			PositionOffset	0 6 0
			#StartJitter		0 6 0

		End

		Bhvr	Behaviors/BuildingExplosionCameraShake.bhvr
		Part	:CoreExplosionFire.part
		Part	:CoreExplosionFire2.part

		Part	World/Events/InfernoExplosionGlow.part

		LifeSpan	10
		

	End

End


Condition
	On 	Time
	Time 	3

	Event
		EName 	Primex
		Type	Local 
		At	Root
		BhvrOverride
			PositionOffset	0 6 0
		End	
#		Bhvr	World/Events/Offset.bhvr
		Part	:FireSparks.part	
		Part	:BlackDeathSmoke.part
		Part	:BlackDeathSteam.part
		LifeSpan	110
	End


End

#############################################################################################
###############################################################################

Condition
	Time 	0
#	Repeat	1
	
	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
#			Scale		2 2 2
			PositionOffset	0 8 -19

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4

			SpinJitter		1.5 1.5 1.5

			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9

			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
		
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R3_gear1a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 8 -19

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4
			
			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R3_gear2a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 8 -19

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4
			
			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R1_gear2a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 8 -19

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4

			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		End

		Geom	_v_destructprtl_R1_gear3a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################
###########################################################

##RoundTwo

#############################################################################################
###############################################################################

Condition
	Time 	0
#	Repeat	1
	
	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
#			Scale		2 2 2
			PositionOffset	0 8 19

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4

			SpinJitter		1.5 1.5 1.5

			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R3_gear1a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 8 19

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4
			
			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R3_gear2a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 8 19

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4
			
			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R1_gear2a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 8 19

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4

			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		End

		Geom	_v_destructprtl_R1_gear3a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################
##############

##RoundThreeMiddle

#############################################################################################
###############################################################################

Condition
	Time 	0
#	Repeat	1
	
	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
#			Scale		2 2 2
			PositionOffset	0 18 0

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4

			SpinJitter		1.5 1.5 1.5

			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R3_gear1a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 18 0

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4
			
			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R3_gear2a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 18 0

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4
			
			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		
		End

		Geom	_v_destructprtl_R1_gear2a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

##################################################################################################

Condition
	Time 	0
#	Repeat	1

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 18 0

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4

			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		End

		Geom	_v_destructprtl_R1_gear3a
		#Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.001
		CDestroy 	0
	End

End

#######################################################

Condition
	Time 	0

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 18 0

			SpinJitter		1.5 1.5 1.5

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4

			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
			startcolor		105 105 105
		End
		Geom	_v_destructprtl_R3_gear2a #_v_destructprtl_ring_01rn
#		Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.01
		CDestroy 	0
	End

End

###################################################################################################

Condition
	Time 	0

	Event
		Type	start
		At	chest

		BhvrOverride
			
			
#			Scale		2 2 2
			PositionOffset	0 18 0

			SpinJitter		0.1 0.5 0.1

			InitialVelocity		0 1.3 0
			InitialVelocityJitter		.4 .6 .4

			StartJitter		9 9 9
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		1.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9

			PhysDebris		1
			PhysDensity		.5
			Gravity	0.05
		
			startcolor		105 105 105
		End

		Geom	_v_destructprtl_R1_gear3a   #_v_destructprtl_ring_03
#		Lifespan	80
		
		#CEvent 	V_COV/PhysicsEnabled/ScrapeSparks.fx
		CThresh 	.01
		CDestroy 	0
	End

End
#######################
##################################################################################################

###########################################################################################


Condition
	Time 	0
	Repeat	9 #12
	Random	1

	Event
		Type	start
		At	chest

		BhvrOverride
			
			PositionOffset	0 6 0
			StartJitter		9 9 9	
						
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		3.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
		End

		Bhvr	:StartOffset.bhvr
		Part	World/Events/fire.part
		Part	World/Events/fire.part
		Part	World/Events/fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
		
		CEvent 	V_COV/PhysicsEnabled/FireCometBurst.fx
		CThresh 	1
		CDestroy 	1
	End

	Event
		Type	start
		At	chest

		BhvrOverride
			
			PositionOffset	0 6 0
			StartJitter		9 9 9	
						
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		3.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
		End

		Bhvr	:StartOffset2.bhvr
		Part	World/Events/fire.part
		Part	World/Events/fire.part
		Part	World/Events/fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
		
		CEvent 	V_COV/PhysicsEnabled/FireCometBurst.fx
		CThresh 	1
		CDestroy 	1
	End

	Event
		Type	start
		At	chest

		BhvrOverride
			
			PositionOffset	0 6 0
			StartJitter		9 9 9	
						
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		3.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
		End

		Bhvr	:StartOffset3.bhvr
		Part	World/Events/fire.part
		Part	World/Events/fire.part
		Part	World/Events/fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
		
		CEvent 	V_COV/PhysicsEnabled/FireCometBurst.fx
		CThresh 	1
		CDestroy 	1
	End

	Event
		Type	start
		At	chest

		BhvrOverride
			
			PositionOffset	0 6 0
			StartJitter		9 9 9	
						
			Physics 		1
			PhysRadius 		1.0
			PhysGravity 		3.5
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		
		End

		Bhvr	:StartOffset4.bhvr
		Part	World/Events/fire.part
		Part	World/Events/fire.part
		Part	World/Events/fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
		
		CEvent 	V_COV/PhysicsEnabled/FireCometBurst.fx
		CThresh 	1
		CDestroy 	1
	End

End

#
#Condition
#	Time 	0
#	Repeat	12
#
#	Event
#		hardwareOnly
#		Type	start
#		At	chest
#
#		BhvrOverride
#			
#			PositionOffset	0 6 0
#			StartJitter		12 3 9
#						
#			Physics 		1
#			PhysRadius 		1.0
#			PhysGravity 		3.5
#			physRestitution 	0.01
#			physSFriction 	 	.9
#			physDFriction 	 	.9
#
#			InitialVelocity		1.6 1 0
#			InitialVelocityJitter	.6 .6 1.6
#		
#		End
#
#		Bhvr	:StartOffset.bhvr
#		Part	World/Events/fire.part
#		Part	World/Events/fire.part
#		Part	World/Events/fireSparks.part
#		Part	:fireSmoke.part
#		Lifespan	80
#		
#		CEvent 	V_COV/PhysicsEnabled/FireCometBurst.fx
#		CThresh 	1
#		CDestroy 	1
#	End
#End

End
