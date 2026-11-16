#####################################
## Arachnos Web Cable Piece - sm 1
#####################################

FxInfo  

Lifespan 300

######## Sounds #########################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Sound flierexplo1 200 200 .93

	End

	Event
		EName 	Sound
		Type	Posit
		At	root	
		Sound molotovhit1 80 80 .86
		#Sound	fireexplo 80 30 .72
		#Sound	firezap 80 30 .76
		#Sound	molotovhit1 80 30 .86
		Sound grexplo2 120.0 120.0 1.0

		LifeSpan	10
			
	End

End


########   Explosion Force   #########################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		100
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End


#################### Fiery Explosion #################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	ExplosionOffset1
		Type	Create
		Inherit	Position Rotation
		Update	Position
		At	Root
		BhvrOverride
			PositionOffset	0 0 -15
		End
		
	End

	Event
		EName 	MetalExplosion
		Type	Local
		At	ExplosionOffset1
		ChildFX	Explosions\MetalExplosion\BlackMetalShrapnelMineExplosion.fx

		#LifeSpan	45
	
	End

End

#Condition
#	On 	time
#	Time 	0
#	Repeat 	8
#
#	Event 
#		EName 	Sparkz
#		Type 	Start
#		At 	ExplosionOffset1
#		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
#		BhvrOverride
#			InitialVelocity		20.0 20.3 20.0
#			InitialVelocityJitter	0.5 0.2 0.5
#		End
#		Lifespan 60
#	End
#End
#
#
#Condition
#	On 	time
#	Time 	0
#	Repeat 	30
#
#	Event 
#		EName 	Sparkzz
#		Type 	Start
#		At 	ExplosionOffset1
#		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
##		BhvrOverride
##			InitialVelocity		20.0 20.3 20.0
##			InitialVelocityJitter	0.5 0.2 0.5
##		End
#		Lifespan 60
#	End
#End


#Condition
#	On	Time
#	Time 	3
#	Repeat	6
#
#	Event
#		EName 	metal
#		Type	Start
#		At	ExplosionOffset1
#		Geom	Metal_Curl
#		Geom	Metal_Flat
#		
#		#Part3	\Explosions\VehicleExplosion\FireBallSpark.part
#		#Part	\Explosions\VehicleExplosion\SmokeExplosion.part
#
#		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
#		BhvrOverride
#			InitialVelocity		0.0 0.3 0.0
#			InitialVelocityJitter	0.5 0.2 0.5
#		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#
#		Lifespan	120
#		LifespanJitter	30
#	End
#
#End


#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		EName 	chunks
#		Type	Local 
#		At	root
#		Part1	Generic\InfernorockBits.part
#		Part2	Generic\InfernorockBitz.part
#		Lifespan	20
#	End
#
#End


######################
## Physics Remains  ##
######################

Condition
	Time 	0

	Event
		Ename	FXPiece01
		Type	Start
		At	Root
		
		Geom	CablePiece_sm_01_FX1
		Bhvr	:CablePieces_Physics.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.0625 0.0625 0.0625
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	150
		LifespanJitter	30
	End

End


#####################################
## Arachnos Web Cable Piece - sm 2
#####################################

FxInfo  

Lifespan 300

######## Sounds #########################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Sound flierexplo1 200 200 .93

	End

	Event
		EName 	Sound
		Type	Posit
		At	root	
		Sound molotovhit1 80 80 .86
		#Sound	fireexplo 80 30 .72
		#Sound	firezap 80 30 .76
		#Sound	molotovhit1 80 30 .86
		Sound grexplo2 120.0 120.0 1.0

		LifeSpan	10
			
	End

End


########   Explosion Force   #########################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		100
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End


#################### Fiery Explosion #################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	ExplosionOffset1
		Type	Create
		Inherit	Position Rotation
		Update	Position
		At	Root
		BhvrOverride
			PositionOffset	0 0 -15
		End
		
	End

	Event
		EName 	MetalExplosion
		Type	Local
		At	ExplosionOffset1
		ChildFX	Explosions\MetalExplosion\BlackMetalShrapnelMineExplosion.fx

		#LifeSpan	45
	
	End

End

#Condition
#	On 	time
#	Time 	0
#	Repeat 	8
#
#	Event 
#		EName 	Sparkz
#		Type 	Start
#		At 	ExplosionOffset1
#		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
#		BhvrOverride
#			InitialVelocity		20.0 20.3 20.0
#			InitialVelocityJitter	0.5 0.2 0.5
#		End
#		Lifespan 60
#	End
#End
#
#
#Condition
#	On 	time
#	Time 	0
#	Repeat 	30
#
#	Event 
#		EName 	Sparkzz
#		Type 	Start
#		At 	ExplosionOffset1
#		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
##		BhvrOverride
##			InitialVelocity		20.0 20.3 20.0
##			InitialVelocityJitter	0.5 0.2 0.5
##		End
#		Lifespan 60
#	End
#End


#Condition
#	On	Time
#	Time 	3
#	Repeat	6
#
#	Event
#		EName 	metal
#		Type	Start
#		At	ExplosionOffset1
#		Geom	Metal_Curl
#		Geom	Metal_Flat
#		
#		#Part3	\Explosions\VehicleExplosion\FireBallSpark.part
#		#Part	\Explosions\VehicleExplosion\SmokeExplosion.part
#
#		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
#		BhvrOverride
#			InitialVelocity		0.0 0.3 0.0
#			InitialVelocityJitter	0.5 0.2 0.5
#		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#
#		Lifespan	120
#		LifespanJitter	30
#	End
#
#End


#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		EName 	chunks
#		Type	Local 
#		At	root
#		Part1	Generic\InfernorockBits.part
#		Part2	Generic\InfernorockBitz.part
#		Lifespan	20
#	End
#
#End


######################
## Physics Remains  ##
######################

Condition
	Time 	0

	Event
		Ename	FXPiece01
		Type	Start
		At	Root
		
		Geom	CablePiece_sm_01_FX1
		Bhvr	:CablePieces_Physics.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	300
		LifespanJitter	30
	End

	Event
		Ename	FXPiece02
		Type	Start
		At	root
				
		Geom	CablePiece_sm_01_FX2
		Bhvr	:CablePieces_Physics.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	300
		LifespanJitter	30
	End

	Event
		Ename	FXPiece03
		Type	Start
		At	Root
				
		Geom	CablePiece_sm_01_FX3
		Bhvr	:CablePieces_Physics.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	300
		LifespanJitter	30
	End

	Event
		Ename	FXPiece04
		Type	Start
		At	Root
				
		Geom	CablePiece_sm_01_FX4
		Bhvr	:CablePieces_Physics.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	300
		LifespanJitter	30
	End

	Event
		Ename	FXPiece05
		Type	Start
		At	Root
				
		Geom	CablePiece_sm_01_FX5
		Bhvr	:CablePieces_Physics.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	300
		LifespanJitter	30
	End


End


End