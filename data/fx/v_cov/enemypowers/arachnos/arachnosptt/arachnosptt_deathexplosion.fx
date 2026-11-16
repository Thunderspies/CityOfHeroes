#########################################################
## Arachnos PTT Death Explosion
#########################################################

FxInfo  

Lifespan 300

##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local 
#		At	chest
#		
#		Sound	flierexplo1 200 30 .93
#	End
#
#	Event
#		EName 	Sound
#		Type	Posit
#		At	base	
#		Sound	molotovhit1 80 30 .86
#		Sound	grexplo2 120.0 30.0 1.0
#
#		LifeSpan	10
#	End
#End

########   Explosion Force   #########################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		100
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1
	End
End

########  Main Explosion Particles   #########################

Condition
	On 	Time
	Time 	0

	Event
		EName 	ExplosionOffset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 0
		End
	End

	Event
		EName 	Flames1
		Type	Local
		At	ExplosionOffset
		ChildFX	med_fire.fx

		Part1	Explosions\VehicleExplosion\FireExplosion2.part
		Part2	Explosions\VehicleExplosion\Inferno.part
		Part	Explosions\VehicleExplosion\ExplosionlightStreams.part
		Part1	Explosions\VehicleExplosion\FireExplosion.part
		Part2	Explosions\VehicleExplosion\Inferno2.part
		Part	Explosions\VehicleExplosion\SmokeExplosion.part
		LifeSpan	15
	End

	Event
		EName 	Explosion
		Type	local
		At	ExplosionOffset
		
		Part1	Explosions\VehicleExplosion\FireExplosion2.part
		Part2	Explosions\VehicleExplosion\Inferno.part
		Part	Explosions\VehicleExplosion\ExplosionlightStreams.part
		Part1	Explosions\VehicleExplosion\FireExplosion.part
		Part2	Explosions\VehicleExplosion\Inferno2.part
		Part	Explosions\VehicleExplosion\SmokeExplosion.part
		
		Lifespan 10
	End

	Event
		EName 	Flash
		Type	local
		At	ExplosionOffset
	
		Part	Explosions\VehicleExplosion\FireExplosionFlash.part
		Part	Explosions\VehicleExplosion\ExplosionStar.part

		Lifespan 15
	End
End


######################################
### Generic Explosion Physic Pieces ##
######################################
 
Condition
	On 	time
	Time 	0
	Repeat 	8

	Event 
		EName 	Sparkz
		Type 	Start
		At 	ExplosionOffset
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	12

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	ExplosionOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat  16

	Event 
		EName 	napalm
		Type 	Local
		At 	ExplosionOffset
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On	Time
	Time 	3
	Repeat	2

	Event
		EName 	metal
		Type	Start
		At	ExplosionOffset
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part	Explosions\VehicleExplosion\SmokeExplosion.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	3
	Repeat	4

	Event
		Ename	parts
		Type	Start
		At	ExplosionOffset

		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part	Explosions\VehicleExplosion\SmokeExplosion.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 2
			PyrRotateJitter		180 180 180
			Scale			1 1 1
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	1.2 0.5 1.2
			Drag			0.01
			physSFriction 	 	0.6
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End
End


##########################################
## Arachnos PTT Generator Physic Pieces ##
##########################################

Condition
	Time 	0

	Event
		Ename	ArachnosPTT_Piece1
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece01
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.0 0.0
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			Spin			0.0625 0.0 0.0
			SpinJitter		0 0.0625 0.0625
			InitialVelocity		0.5 0.25 0.5
			InitialVelocityJitter	0.125 0.125 0.125
			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		Ename	ArachnosPTT_Piece2
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece02
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.0 0.0
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			Spin			-0.0625 0.0 0.0
			SpinJitter		0 0.0625 0.0625
			InitialVelocity		-0.5 0.25 -0.5
			InitialVelocityJitter	0.125 0.125 0.125
			physRestitution 	0.25
		End
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		Ename	ArachnosPTT_Piece3
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece03
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-0.404 11.382 -0.877
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			Spin			0 0 0
			SpinJitter		0.125 0.0625 0.125
			InitialVelocity		0 1 0
			InitialVelocityJitter	0.125 0.125 0.125
			physRestitution 	0.25
		End
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		Ename	ArachnosPTT_Piece4
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece04
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-0.421 4.758 -0.869
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			Spin			0 0 0
			SpinJitter		0.125 0.0625 0.125
			InitialVelocity		0.0 0.75 0.0
			InitialVelocityJitter	0.125 0.125 0.125
			physRestitution 	0.25
		End
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		Ename	ArachnosPTT_Piece7
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece07
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-0.121 5.701 1.87
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			Spin			0 0 0
			SpinJitter		0.125 0.0625 0.125
			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.125 0.125 0.125
			physRestitution 	0.25
		End
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		Ename	ArachnosPTT_Piece5
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece05
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-0.121 2.279 1.87
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			Spin			0 0 0
			SpinJitter		0.125 0.0625 0.125
			InitialVelocity		0 0.5 0
			InitialVelocityJitter	0.125 0.125 0.125
			physRestitution 	0.25
		End
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		Ename	ArachnosPTT_Piece6
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece06
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		2.196 2.926 0.171
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			SpinJitter		0.125 0.0625 0.125
			InitialVelocity		0 1 0
			InitialVelocityJitter	0.125 0.125 0.125
			physRestitution 	0.25
		End
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		Ename	ArachnosPTT_Piece8
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece08
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-2.719 3.584 0.802
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			Spin			0 0 0
			SpinJitter		0.125 0.0625 0.125
			InitialVelocity		0 1 0
			InitialVelocityJitter	0.125 0.25 0.125
			physRestitution 	0.25
		End
		
		Lifespan	240
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	0
	Repeat	1

	Event
		Ename	ArachnosPTT_Piece9
		Type	Start
		At	ExplosionOffset
				
		Geom	exp_arachnosPTT_Piece09
		Bhvr	Explosions\VehicleExplosion\SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.0 0.0
			Scale			1 1 1
			PyrRotateJitter		0 0 0
			Spin			0 0 0
			SpinJitter		0.125 0.0625 0.125
			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.5 0.25 0.5
			physRestitution 	0.25
		End
		
		Lifespan	240
		LifespanJitter	30
	End

End

##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##

End