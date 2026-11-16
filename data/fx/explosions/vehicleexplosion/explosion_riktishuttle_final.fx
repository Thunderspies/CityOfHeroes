#########################################################
## Rikti Shuttle - Explosion/Death FX
#########################################################

FxInfo  

Flags InheritAnimScale

Lifespan 300

## SOUND FX #######################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local 
#		At	Chest
#		Sound	flierexplo1 200 30 .93
#	End
#End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		256
			PhysForcePower		500
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End
End

## CAMERA SHAKE #######################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	CameraShake
		Type	Start
		At	Chest
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.75	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.01	#how long shake lasts
			ShakeRadius    768	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	TestSphere
		Type	Local
		At	Hips
		Bhvr	:RiktiShuttle\Explosion_Final_SphereScaler.bhvr
		Geom	GEO_WhiteSphere
		Lifespan	30
	End
End


## EXPLOSION #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash
		Type	Local
		At	Origin
		Part	:RiktiShuttle\Explosion_Final_Flash1.part
		Part	:RiktiShuttle\Explosion_Final_Flash2.part
		Lifespan	10
	End

#	Event
#		EName 	Flames1
#		Type	Local
#		At	Origin
#		Part	:RiktiShuttle\Explosion_Final_Smoke.part
#		Part	:RiktiShuttle\Explosion_Final_Smoke2.part
#		Part2	:RiktiShuttle\Explosion_Final_Plasma.part
#		Part2	:RiktiShuttle\Explosion_Final_Plasma2.part
#		#Part1	:RiktiShuttle\FireExplosion2.part
#		#Part2	:RiktiShuttle\Inferno.part
#		LifeSpan	4
#	End

	Event
		EName 	Flames1
		Type	Local
		At	Origin
		Part	:RiktiShuttle\Explosion_Final_Streaks1.part
		Part	:RiktiShuttle\Explosion_Final_Streaks2.part
		LifeSpan	10
	End
End

## NOVA #######################################################
 
Condition
	On 	Time
	Time 	0

	Event
		EName 	Shockwave
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	22.5 0 22.5
		End
		Part	:RiktiShuttle\Explosion_Final_Shockwave1.part
		Lifespan	10
	End

	Event
		EName 	Prime1
		Type	Local 
		At	Shockwave
		bhvr	:RiktiShuttle\Explosion_Final_ShockwaveScaler1.bhvr
		Part	:RiktiShuttle\Explosion_Final_ShockwaveTrails1.part
		Part	:RiktiShuttle\Explosion_Final_ShockwaveSmoke1.part
		Part	:RiktiShuttle\Explosion_Final_ShockwaveSmoke2.part
		LifeSpan	90
	End

	Event
		EName	ring
		Type	start
		At	Shockwave
		bhvr	:RiktiShuttle\Explosion_Final_ShockwaveScaler2.bhvr
		geom	MegaRing2
		LifeSpan	30
	End
End

## SPARKS #######################################################

#Condition
#	On 	time
#	Time 	0
#	Repeat 	8
#
#	Event 
#		EName 	Sparks
#		Type 	Start
#		At 	Origin
#		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
#		Lifespan	60
#		#LifespanJitter 	30
#	End
#End
#
#Condition
#	On 	time
#	Time 	0
#	Repeat  4
#
#	Event 
#		EName 	FierySparks
#		Type 	Start
#		At 	Origin
#		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
#		Lifespan	90
#		LifespanJitter 	30
#	End
#End

## FLAMING TENDRILS #######################################################

Condition
	On	Time
	Time 	5
	Repeat	8

	Event
		Ename	parts
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	4 2 4
			Scale			1 1 1
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			Drag			0.03125
			StartColor		0 64 16
		End
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Part3	:FireBallSpark.part
		Part	:Tendrils_SmokeTrails1.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		Lifespan	120
		LifespanJitter	30
	End
End


Condition
	On	Time
	Time 	10
	Repeat	8

	Event
		Ename	parts
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	4 2 4
			Scale			0.125 0.125 0.125
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.03125
			StartColor		0 64 16
		End
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Part3	:FireBallSpark.part
		Part	:Tendrils_SmokeTrails2.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		Lifespan	120
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	5
	Repeat	8

	Event
		EName 	metal
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	2 1 2
			Scale			0.125 0.125 0.125
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			Drag			0.03125
			StartColor		0 64 16
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		Part3	:FireBallSpark.part
		Part	:Tendrils_SmokeTrails1.part
		Lifespan	120
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	10
	Repeat	8

	Event
		EName 	metal
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	4 2 4
			Scale			0.125 0.125 0.125
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.03125
			StartColor		0 64 16
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		Part3	:FireBallSpark.part
		Part	:Tendrils_SmokeTrails2.part
		Lifespan	120
		LifespanJitter	30
	End
End

## METAL DEBRIS #######################################################

Condition
	On 	time
	Time 	5

	Event 
		EName 	MetalDebris
		Type 	Start
		At 	Origin
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary_MetalDebris.fx
	End
End

#######################################################################

End			