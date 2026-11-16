#########################################################
## Rikti Shuttle - Explosion/Death FX
#########################################################

FxInfo  

Lifespan 300

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

#	Event
#		Type	Local 
#		At	Chest
#		Sound	flierexplo1 200 30 .93
#	End

	Event
		Type	Local 
		At	Chest
		Sound molotovhit1 80 80 .86
		#Sound	fireexplo 80 30 .72
		#Sound	firezap 80 30 .76
		#Sound	molotovhit1 80 30 .86
		Sound grexplo2 120.0 120.0 1.0
		LifeSpan	10
	End
End

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
			PhysForceRadius		100
			PhysForcePower		500
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End
End

## CAMERA SHAKE #######################################################

#Condition
#	On 	Time
#	Time 	2
#
#	Event
#		EName 	CameraShake
#		Type	Start
#		At	Chest
#		Bhvr	Behaviors/CameraShake01.bhvr
#		BhvrOverride
#			Shake          0.75	#How much to shake the camera when event with this bhvr is created
#			ShakeFallOff   0.0375	#how long shake lasts
#			ShakeRadius    512	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
#		End
#	End
#End


## EXPLOSION #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	EngineOffset
		Type	Local
		At	Col_R
		BhvrOverride
			PositionOffset 0 0 -16
		End
	End

	Event
		Type	Local
		At	EngineOffset
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary.fx
	End

##	Event
##		EName 	Flash
##		Type	local
##		At	EngineOffset
##		Part	:FireExplosionFlash.part
##		Part	:ExplosionStar.part
##		Lifespan	10
##	End
##
##	Event
##		EName 	Fire
##		Type	local
##		At	EngineOffset
##		Part1	:FireBallTargetSteam.part
##		Part2	:FireBallTargetFlame2.part
##		Part3	:FireBallSpark.part
##		ChildFX	med_fire.fx
##		LifeSpan	45
##	End
##
##	Event
##		EName 	Flames1
##		Type	Local
##		At	EngineOffset
##		ChildFX	med_fire.fx
##		Part1	:FireExplosion2.part
##		Part2	:Inferno.part
##		Part	:ExplosionlightStreams.part
##		Part1	:FireExplosion.part
##		Part2	:Inferno2.part
##		Part	:SmokeExplosion.part
##		LifeSpan	45
##	End
##
##	Event
##		EName 	Smoke
##		Type	local
##		At	EngineOffset
##		Part	:RiktiShuttle\SmokeTrail1.part
##		Lifespan	150
##	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	chunks
		Type	Local 
		At	EngineOffset
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Lifespan	20
	End
End

## SPARKS #######################################################
 
Condition
	On 	time
	Time 	0
	Repeat 	16

	Event 
		EName 	Sparks
		Type 	Start
		At 	EngineOffset
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan	60
		#LifespanJitter 	30
	End
End

Condition
	On 	time
	Time 	0
	Repeat  8

	Event 
		EName 	FierySparks
		Type 	Local
		At 	EngineOffset
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan	90
		LifespanJitter 	30
	End
End

## FLAMING TENDRILS #######################################################

Condition
	On	Time
	Time 	3
	Repeat	4

	Event
		EName 	metal
		Type	Start
		At	EngineOffset
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.5 0.375 0.5
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			Drag			0.01
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
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
		At	EngineOffset
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	1.0 0.5 1.5
			Scale			1 1 1
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5
			Drag			0.01
		End
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
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
		Type 	Local
		At 	EngineOffset
		ChildFX Explosions\VehicleExplosion\RiktiShuttle\Explosion_Secondary_MetalDebris.fx
	End
End

#######################################################################

End			