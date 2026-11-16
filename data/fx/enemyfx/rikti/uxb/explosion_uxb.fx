#########################################################
## Rikti Heavy Assault Suit - Explosion/Death FX
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
		Ename	BaseOffset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 4 0
		End
	End
End


## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	BaseOffset
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
		At	BaseOffset
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
		At	BaseOffset
		Bhvr	:Explosion\Explosion_SphereScaler.bhvr
		Geom	GEO_WhiteSphere
		Lifespan	10
	End
End


## EXPLOSION #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash
		Type	Local
		At	BaseOffset
		Part	:Explosion\Explosion_Flash1.part
		Part	:Explosion\Explosion_Flash2.part
		Lifespan	10
	End

	Event
		EName 	Flames1
		Type	Local
		At	BaseOffset
		Part	:Explosion\Explosion_Smoke.part
		Part2	:Explosion\Explosion_Plasma.part
		Part2	:Explosion\Explosion_Plasma2.part
		LifeSpan	4
	End

	Event
		EName 	Flames1
		Type	Local
		At	BaseOffset
		Part	:Explosion\Explosion_Streaks1.part
		Part	:Explosion\Explosion_Streaks2.part
		LifeSpan	5
	End
End

## NOVA #######################################################
 
Condition
	On 	Time
	Time 	0

	Event
		EName 	Shockwave
		Type	Local
		At	BaseOffset
		BhvrOverride
			PyrRotateJitter	0 0 0
		End
		Lifespan	10
	End

	Event
		EName 	Prime1
		Type	Local 
		At	BaseOffset
		bhvr	:Explosion\Explosion_ShockwaveScaler1.bhvr
		Part	:Explosion\Explosion_ShockwaveTrails1.part
		Part	:Explosion\Explosion_ShockwaveSmoke1.part
		Part	:Explosion\Explosion_ShockwaveSmoke2.part
		LifeSpan	15
	End

	Event
		EName	ring
		Type	start
		At	BaseOffset
		bhvr	:Explosion\Explosion_ShockwaveScaler2.bhvr
		geom	MegaRing2
		Part	:Explosion\Explosion_Shockwave1.part
		LifeSpan	15
	End
End

## SPARKS #######################################################

Condition
	On 	time
	Time 	0
	Repeat 	8

	Event 
		EName 	Sparks
		Type 	Start
		At 	BaseOffset
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan	60
		#LifespanJitter 	30
	End
End

Condition
	On 	time
	Time 	0
	Repeat  4

	Event 
		EName 	FierySparks
		Type 	Start
		At 	BaseOffset
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan	90
		LifespanJitter 	30
	End
End

## METAL DEBRIS #######################################################

Condition
	On	Time
	Time 	0
	Repeat	6

	Event
		EName 	metal
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 1 0.0
			InitialVelocityJitter	1 0.25 1
			Scale			0.0625 0.0625 0.0625
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.005
			StartColor		0 64 16
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		Lifespan	90
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	0
	Repeat	6

	Event
		EName 	metal
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 2 0.0
			InitialVelocityJitter	2 0.5 2
			Scale			0.03125 0.03125 0.03125
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.005
			StartColor		0 64 16
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		Lifespan	90
		LifespanJitter	30
	End
End

#######################################################################

End			