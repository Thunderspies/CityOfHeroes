#########################################################
## Rikti Transport Shuttle - Persistent FX
#########################################################

FxInfo

Lifespan 300

Flags InheritAnimScale

## SOUND FX #######################################################

#Condition
#
#	On 	Time	
#	Time	18
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans4 80 30 .88
#	End
#
#End
#
#Condition
#
#	On 	Time	
#	Time	70
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans3 80 30 .78
#	End
#
#End


## PERSISTENT EFFECTS #######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Hookup
		Type	Local
#		Type	Create
#		Inherit	All
#		Update	All
		At	Hips
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	FX_Thrusters1
		Type	local
		At	UArmL
		BhvrOverride
			PositionOffset		0 0 -4
		End
		Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:SmallThrusterCore.part
		Part	:SmallThrusterCoreFlames.part
	End

	Event
		Ename	FX_Thrusters2
		Type	local
		At	LArmL
		BhvrOverride
			PositionOffset		0 0 -4
		End
		Part	:SmallThrusterCoreWhiteHottLicks.part
		Part	:SmallThrusterCore.part
		Part	:SmallThrusterCoreFlames.part
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	FX_StarboardExhaust1
		Type	local
		At	ULegR
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
		Lifespan	155
	End

	Event
		Ename	FX_StarboardExhaust2
		Type	local
		At	LLegR
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
		Lifespan	155
	End

	Event
		Ename	FX_StarboardExhaust3
		Type	local
		At	FootR
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
		Lifespan	155
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	FX_PortExhaust1
		Type	local
		At	ULegL
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
		Lifespan	155
	End

	Event
		Ename	FX_PortExhaust2
		Type	local
		At	LLegL
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
		Lifespan	155
	End

	Event
		Ename	FX_PortExhaust3
		Type	local
		At	FootL
		Part	:ExhaustPort_Glow1.part
		Part	:ExhaustPort_Glow2.part
		Lifespan	155
	End
End


## GEOMETRY #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	RiktiShuttle_AltPivots
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_AltPivots
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	RiktiShuttle_ReactorShield
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_ReactorShield
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	RiktiShuttle_Antenna1
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_Antenna1
	End

	Event
		Ename	RiktiShuttle_Antenna2
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_Antenna2
	End

	Event
		Ename	RiktiShuttle_Antenna3
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_Antenna3
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	RiktiShuttle_StarboardExhaustRing
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_StarboardExhaustRing
	End

	Event
		Ename	RiktiShuttle_PortExhaustRing
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_PortExhaustRing
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	RiktiShuttle_DeckPlating1
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_DeckPlating1
	End

	Event
		Ename	RiktiShuttle_ForwardArmorCoupling
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_ForwardArmorCoupling
	End

	Event
		Ename	RiktiShuttle_BellyPlating1
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_BellyPlating1
	End

	Event
		Ename	RiktiShuttle_ForwardArials
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_ForwardArials
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	RiktiShuttle_DeckPlating2
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_DeckPlating2
	End

	Event
		Ename	RiktiShuttle_BellyPlating2
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_BellyPlating2
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	RiktiShuttle_DeckPlating3
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_DeckPlating3
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	RiktiShuttle_Hull
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_Hull
	End
End

## STARBOARD ENGINE DEBRIS #######################################################

Condition
	On	Time
	Time 	0

	Event
		EName 	metal
		Type	Start
		At	RiktiShuttle_AltPivots
		AltPiv	1
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0 0 0
			InitialVelocityJitter	0.0 0.0 0.0
			Scale			1.0 1.0 1.0
			Spin			0.0 0.01 0.0
			SpinJitter		0.03125 0.0625 0.03125 #0.03125 0.03125 0.03125
			Gravity			0.01
			Drag			0.0
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_StabilizerStarboard
		Lifespan	120
		LifespanJitter	30
	End

	Event
		EName 	metal
		Type	Start
		At	RiktiShuttle_AltPivots
		AltPiv	2
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0 0 0
			InitialVelocityJitter	0.0 0.0 0.0
			Scale			1.0 1.0 1.0
			Spin			0.0 0.01 0.0
			SpinJitter		0.03125 0.0625 0.03125 #0.03125 0.03125 0.03125
			Gravity			0.01
			Drag			0.0
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_StabilizerStarboard_Tip
		Lifespan	120
		LifespanJitter	30
	End
End

## SECONDARY EXPLOSION 1 - REACTOR CORE #######################################################


Condition

	On 	Time	
	Time	35

	Event
		Type Local
		At origin
		Sound boomexplo2 500 500 .9
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName	Explosion_ReactorCore
		Type	Local
		At	Hookup
		BhvrOverride
			PositionOffset	0.0 -6 -54
		End
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	36

	Event
		EName 	CameraShake
		Type	Local
		At	Hookup
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.5	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.0375	#how long shake lasts
			ShakeRadius    256	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

Condition
	On 	Time
	Time 	36

	Event
		Type	local
		At	Hookup
		ChildFX	Explosions\VehicleExplosion\RiktiShuttle\Explosion_ReactorShield.fx
	End

	Event
		Type	Local
		At	Explosion_ReactorCore
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary.fx
	End

	Event 
		EName 	MetalDebris
		Type 	Start
		At 	Explosion_ReactorCore
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary_MetalDebris.fx
	End

	Event
		EName	RiktiShuttle_ReactorShield
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_Antenna1
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_Antenna2
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_Antenna3
		Type	Destroy
	End
End


## SECONDARY EXPLOSION 2 - PORT ENGINE #######################################################

Condition

	On 	Time	
	Time	70

	Event
		Type Local
		At origin
		Sound exploburst2 500 500 .8
	End

End

Condition
	On 	Time
	Time 	70

	Event
		EName	Explosion_PortEngine
		Type	Local
		At	Col_L
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Lifespan	30
	End

	Event
		Ename	RiktiShuttle_Hull_Destroyed
		Type	Local
		At	Hookup
		Geom	RiktiShuttle_Hull_Destroyed
	End
End

Condition
	On 	Time
	Time 	72

	Event
		EName 	CameraShake
		Type	Local
		At	Explosion_PortEngine
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.5	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.0375	#how long shake lasts
			ShakeRadius    256	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

Condition
	On 	Time
	Time 	70

	Event
		Type	Local
		At	Explosion_PortEngine
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary.fx
	End

	Event 
		EName 	MetalDebris
		Type 	Start
		At 	Explosion_PortEngine
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary_MetalDebris.fx
	End

	Event
		EName	RiktiShuttle_Hull
		Type	Destroy
	End

	Event
		EName	FX_Thrusters1
		Type	Destroy
	End

	Event
		EName	FX_Thrusters2
		Type	Destroy
	End

	Event
		EName	FX_StarboardExhaust1
		Type	Destroy
	End

	Event
		EName	FX_StarboardExhaust2
		Type	Destroy
	End

	Event
		EName	FX_StarboardExhaust3
		Type	Destroy
	End

	Event
		EName	FX_PortExhaust1
		Type	Destroy
	End

	Event
		EName	FX_PortExhaust2
		Type	Destroy
	End

	Event
		EName	FX_PortExhaust3
		Type	Destroy
	End
End


## SECONDARY EXPLOSION 3 - Deck Plating 1 #######################################################

Condition

	On 	Time	
	Time	121

	Event
		Type Local
		At origin
		Sound flashexplo 500 500 .8
	End

End


Condition
	On 	Time
	Time 	121

	Event
		EName	Explosion_3
		Type	local
		At	Hookup
		BhvrOverride
			PositionOffset	0.0 -8 88
		End
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	123

	Event
		EName 	CameraShake
		Type	local
		At	Explosion_3
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.5	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.0375	#how long shake lasts
			ShakeRadius    256	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

Condition
	On 	Time
	Time 	121

	Event
		Type	local
		At	Explosion_3
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary.fx
	End

	Event 
		EName 	MetalDebris
		Type 	Start
		At 	Explosion_3
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary_MetalDebris.fx
	End

	Event
		EName	RiktiShuttle_DeckPlating1
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_ForwardArmorCoupling
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_BellyPlating1
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_ForwardArials
		Type	Destroy
	End
End

Condition
	On	Time
	Time 	121

#	Event
#		EName 	TrackTarget
#		Type	Local
#		At	RiktiShuttle_AltPivots
#		AltPiv	10
#	End

	Event
		EName 	metal
		Type	Local
		At	RiktiShuttle_AltPivots
		AltPiv	8
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0.0 0.625 0.0
			InitialVelocityJitter	0.0625 0.0625 0.0625
			Scale			1.0 1.0 1.0
			Spin			-0.01 0.0 0.0
			SpinJitter		0.005 0.005 0.005
			Gravity			0.0075
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_DeckPlating1_Destroyed
		Lifespan	300
	End

	Event
		EName 	metal
		Type	Local
		At	RiktiShuttle_AltPivots
		AltPiv	9
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0.0 -0.25 0.0
			InitialVelocityJitter	0.0625 0.0625 0.0625
			Scale			1.0 1.0 1.0
			Spin			0.01 0.0 0.0
			SpinJitter		0.005 0.005 0.005
			Gravity			0.01
			Drag			0.0
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_BellyPlating1_Destroyed
		Lifespan	300
	End
End


## SECONDARY EXPLOSION 4 - Deck Plating 2 #######################################################


Condition

	On 	Time	
	Time	166

	Event
		Type Local
		At origin
		Sound boom_explo4 500 500 .8
	End

End

Condition
	On 	Time
	Time 	166

	Event
		EName	Explosion_4
		Type	Local
		At	Hookup
		BhvrOverride
			PositionOffset	8 0 58
		End
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	168

	Event
		EName 	CameraShake
		Type	local
		At	Explosion_4
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.5	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.0375	#how long shake lasts
			ShakeRadius    256	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

Condition
	On 	Time
	Time 	166

	Event
		Type	Local
		At	Explosion_4
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary.fx
	End

	Event 
		EName 	MetalDebris
		Type 	Start
		At 	Explosion_4
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary_MetalDebris.fx
	End

	Event
		EName	RiktiShuttle_DeckPlating2
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_BellyPlating2
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_StarboardExhaustRing
		Type	Destroy
	End

	Event
		EName	RiktiShuttle_PortExhaustRing
		Type	Destroy
	End
End

Condition
	On	Time
	Time 	166

	Event
		EName 	metal
		Type	Local
		At	RiktiShuttle_AltPivots
		AltPiv	5
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0.0 0.625 0.0
			InitialVelocityJitter	0.0625 0.0625 0.0625
			Scale			1.0 1.0 1.0
			Spin			-0.01 0.0 0.0
			SpinJitter		0.005 0.005 0.005
			Gravity			0.0075
			Drag			0.0
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_DeckPlating2_Destroyed
		Lifespan	300
	End

	Event
		EName 	metal
		Type	Local
		At	RiktiShuttle_AltPivots
		AltPiv	7
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0.0 0.625 0.0
			InitialVelocityJitter	0.0625 0.0625 0.0625
			Scale			1.0 1.0 1.0
			Spin			-0.01 0.0 0.0
			SpinJitter		0.005 0.005 0.005
			Gravity			0.0075
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_BellyPlating2_Destroyed
		Lifespan	300
	End
End

Condition
	On	Time
	Time 	166

	Event
		EName 	metal
		Type	Start
		At	RiktiShuttle_AltPivots
		AltPiv	3
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0.0 -0.0625 0.0
			InitialVelocityJitter	0.03125 0.0625 0.03125
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
			Gravity			0.04
			Drag			0.01
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_StarboardExhaustRing_Destroyed
		Lifespan	120
		LifespanJitter	30
	End

	Event
		EName 	metal
		Type	Start
		At	RiktiShuttle_AltPivots
		AltPiv	6
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0.0 -0.0625 0.0
			InitialVelocityJitter	0.03125 0.0625 0.03125
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
			Gravity			0.04
			Drag			0.01
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_PortExhaustRing_Destroyed
		Lifespan	120
		LifespanJitter	30
	End
End


## SECONDARY EXPLOSION 5 - Deck Plating 3 #######################################################


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound HugeRockExplo2 500 500 .7
	End

End


Condition
	On 	Time
	Time 	196

	Event
		EName	Explosion_5
		Type	Local
		At	Hookup
		BhvrOverride
			PositionOffset	16 8 16
		End
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	198

	Event
		EName 	CameraShake
		Type	local
		At	Explosion_5
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.5	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.0375	#how long shake lasts
			ShakeRadius    256	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

Condition
	On 	Time
	Time 	196

	Event
		Type	Local
		At	Explosion_5
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary.fx
	End

	Event 
		EName 	MetalDebris
		Type 	Start
		At 	Explosion_5
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary_MetalDebris.fx
	End

	Event
		EName	RiktiShuttle_DeckPlating3
		Type	Destroy
	End
End

Condition
	On	Time
	Time 	196

	Event
		EName 	metal
		Type	Local
		At	RiktiShuttle_AltPivots
		AltPiv	4
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		0 0 0
			PositionOffset		0 0 0
			InitialVelocity		0.0 0.625 0.0
			InitialVelocityJitter	0.0625 0.0625 0.0625
			Scale			1.0 1.0 1.0
			Spin			-0.01 0.0 0.0
			SpinJitter		0.005 0.005 0.005
			Gravity			0.0075
			StartColor		255 255 255
		End
		Geom	RiktiShuttle_DeckPlating3_Destroyed
		Lifespan	300
	End
End


## CLEAN UP BEFORE FINAL EXPLOSION #######################################################

Condition
	On 	Time
	Time 	270

	Event
		EName	ALL
		Type	Destroy
	End
End

#########################################################

End		