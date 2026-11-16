#########################################################
## Rikti UXB - Death FX (defeated before detonation)
#########################################################

FxInfo  

Lifespan 150

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		Sound car_explo1 80 80 .9
	End
End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 2 0
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		800
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

## MAIN EXPLOSION #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	base
		Type	Create
		Inherit	Position Rotation
		Update	Position
		At	Root
		BhvrOverride
			PositionOffset	0 3 0
		End
	End

	Event
		EName 	Sound
		Type	Posit
		At	base	
		Sound molotovhit1 80 80 .86
		Sound grexplo2 120.0 120.0 1.0
		LifeSpan	10
	End

	Event
		EName 	Fire
		Type	local
		At	base
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		Part2	Explosions\VehicleExplosion\FireBallTargetFlame2.part
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		ChildFX	med_fire.fx
		LifeSpan	20
	End

	Event
		EName 	Offset1
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 0 3
		End
		LifeSpan	25
	End

	Event
		EName 	Flames1
		Type	Local
		At	Offset1
		ChildFX	med_fire.fx
		LifeSpan	25
	End

	Event
		EName 	Offset2
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 0 -3
		End
		LifeSpan	25
	End

	Event
		EName 	Flames2
		Type	local
		At	Offset2
		ChildFX	med_fire.fx
		LifeSpan	25
	End

	Event
		EName 	Smoke
		Type	local
		At	base
		Part	Explosions\VehicleExplosion\SmokeExplosion.part
		Lifespan	15
	End

	Event
		EName 	Explosion
		Type	local
		At	base
		Part1	Explosions\VehicleExplosion\FireExplosion2.part
		Part2	Explosions\VehicleExplosion\Inferno.part
		Part	Explosions\VehicleExplosion\ExplosionlightStreams.part
		Lifespan	8
	End

	Event
		EName 	Explosion2
		Type	local
		At	base
		Part1	Explosions\VehicleExplosion\FireExplosion.part
		Part2	Explosions\VehicleExplosion\Inferno2.part
		Lifespan	12
	End

	Event
		EName 	flat
		Type	Local 
		At	Root
		Part2	Explosions\VehicleExplosion\FlatInferno.part
		Part	Explosions\VehicleExplosion\FlatExplosionStar.part
		LifeSpan	10
	End

	Event
		EName 	Flash
		Type	local
		At	base
		Part	Explosions\VehicleExplosion\FireExplosionFlash.part
		Part	Explosions\VehicleExplosion\ExplosionStar.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	chunks
		Type	Local 
		At	base
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Lifespan	10
	End
End

########   Generic Explosion Physics Particles   ###################
 
Condition
	On	Time
	Time	0
	Repeat 	7

	Event 
		EName 	Sparkz
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan	60
	End
	
	Event 
		EName 	Sparkz2
		HardwareOnly
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\HotSparkUP.fx
		Lifespan	60
	End
End

Condition
	On	Time
	Time	0
	Repeat 	15

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan	60
	End
End


Condition
	On	Time
	Time	0
	Repeat  7
	RepeatJitter	3

	Event 
		EName 	napalm
		Type 	Local
		At 	base
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan	70
		LifespanJitter 	20
	End
End

Condition
	On	Time
	Time 	3
	Repeat	6

	Event
		EName 	metal
		Type	Start
		At	base
		Geom	Metal_Curl
		Geom	Metal_Flat
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		Lifespan	120
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	3
	Repeat	9

	Event
		Ename	parts2
		HardwareOnly
		Type	Start
		At	base
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 2
			PyrRotateJitter		180 180 180
			Scale			0.8 0.8 0.8
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	1.2 0.8 1.2
			Drag			0.01
			physSFriction 	 	0.6
			PhysDebris		1
		End
		Lifespan	10
	End
End

Condition
	On	Time
	Time 	3
	Repeat	8

	Event
		Ename	parts3
		HardwareOnly
		Type	Start
		At	base
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		Part2	Explosions\VehicleExplosion\FireBallTargetFlame2.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10
		Geom	Widget_12
		Geom	Widget_13		
		Geom	Widget_14
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 2
			PyrRotateJitter		180 180 180
			Scale			.5 .5 .5
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	1.2 0.8 1.2
			Drag			0.01
			physSFriction 	 	0.6
			PhysDebris		1
		End
		Lifespan	60
		LifespanJitter	30
	End
End

## FX GEOS #######################################################

#Condition
#	On	Time
#	Time 	0
#
#	Event
#		EName 	EngineFire
#		Type	Local
#		At	Husk
#		BhvrOverride
#			PositionOffset	0 0 4
#		End
#		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
#		Part2	Explosions\VehicleExplosion\FireBallTargetFlame2.part
#		Part3	Explosions\VehicleExplosion\FireBallSpark.part
#		Part	Explosions\VehicleExplosion\SmokeExplosion.part
#	End
#End

Condition
	On	Time
	Time 	0

	Event
		Ename	UXB_DebrisHookup
		Type	Start
		At	Root
		Geom	UXB_DebrisHookup
		ChildFX	med_fire.fx
		Lifespan	90
		LifespanJitter	30
	End

	Event
		Ename	Chunk_01
		Type	Start
		At	UXB_DebrisHookup
		AltPiv	1
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			Spin			0.000 0.000 0.000
			SpinJitter		0.125 0.125 0.125
			InitialVelocity		0.000 1.500 0.000
			InitialVelocityJitter	0.125 0.500 0.125
			PhysDebris		1
		End
		Geom	rikti_UXB_chunk_01
		Part	Explosions\VehicleExplosion\SmokeExplosion.part
		Lifespan	120
		LifespanJitter	30
	End


	Event
		Ename	Chunk_02
		Type	Start
		At	UXB_DebrisHookup
		AltPiv	2
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35
			InitialVelocity		0.000 0.750 0.000
			InitialVelocityJitter	0.125 0.250 0.125
			PhysDebris		1
		End
		Geom	rikti_UXB_chunk_02
		Part	Explosions\VehicleExplosion\SmokeExplosion.part
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	Chunk_03
		Type	Start
		At	UXB_DebrisHookup
		AltPiv	3
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35
			InitialVelocity		0.5 0.6 0.0
			InitialVelocityJitter	0.3 0.4 0.2
			PhysDebris		1
		End
		Geom	rikti_UXB_chunk_03
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	Chunk_04
		Type	Start
		At	UXB_DebrisHookup
		AltPiv	4
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35
			InitialVelocity		-0.5 0.6 0.0
			InitialVelocityJitter	0.3 0.4 0.2
			PhysDebris		1
		End
		Geom	rikti_UXB_chunk_04
		#Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	Chunk_05
		Type	Start
		At	UXB_DebrisHookup
		AltPiv	5
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			InitialVelocity		0.0 0.5 -0.5
			InitialVelocityJitter	0.2 0.4 0.3
			PhysDebris		1
		End
		Geom	rikti_UXB_chunk_05
		#Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	Chunk_06
		Type	Start
		At	UXB_DebrisHookup
		AltPiv	6
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35
			InitialVelocity		0.0 0.4 0.6
			InitialVelocityJitter	0.2 0.3 0.3
			PhysDebris		1
		End
		Geom	rikti_UXB_chunk_06
		#Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	Chunk_07
		Type	Start
		At	UXB_DebrisHookup
		AltPiv	7
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35
			InitialVelocity		0.0 0.4 -0.6
			InitialVelocityJitter	0.2 0.3 0.3
			PhysDebris		1
		End
		Geom	rikti_UXB_chunk_07
		#Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	Chunk_08
		Type	Start
		At	UXB_DebrisHookup
		AltPiv	8
		Bhvr	Explosions\VehicleExplosion\PhysicsDebrisFade.bhvr
		BhvrOverride
			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35
			InitialVelocity		0.3 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.2
			PhysDebris		1
		End
		Geom	rikti_UXB_chunk_08
		#Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Lifespan	120
		LifespanJitter	30
	End
End

#########################################################

End