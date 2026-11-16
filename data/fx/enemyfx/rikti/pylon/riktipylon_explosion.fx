#########################################################
## Rikti Heavy Assault Suit - Explosion/Death FX
#########################################################

FxInfo  

Flags InheritAnimScale

Lifespan 300

#SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Chest
		Sound FlierExplo1 120 120 .73
	End
End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	BaseOffset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 3 0
		End
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	MidOffset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 12 0
		End
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	TopOffset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 24 0
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
		At	TopOffset
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
	Time 	4

	Event
		EName 	CameraShake
		Type	Start
		At	TopOffset
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.75	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.025	#how long shake lasts
			ShakeRadius    768	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName	TestSphere
		Type	Local
		At	TopOffset
		Bhvr	:Explosion\Explosion_SphereScaler.bhvr
		Geom	GEO_WhiteSphere
		Lifespan	10
	End
End


## EXPLOSION #######################################################

Condition
	On 	Time
	Time 	6

	Event
		EName 	Flash
		Type	Local
		At	TopOffset
		Part	:Explosion\Explosion_Flash1.part
		Part	:Explosion\Explosion_Flash2.part
		Lifespan	3
	End

	Event
		EName 	Flames1
		Type	Local
		At	TopOffset
		Part	:Explosion\Explosion_Smoke.part
		Part2	:Explosion\Explosion_Plasma.part
		Part2	:Explosion\Explosion_Plasma2.part
		LifeSpan	3
	End

	Event
		EName 	Flames1
		Type	Local
		At	TopOffset
		Part	:Explosion\Explosion_Streaks1.part
		Part	:Explosion\Explosion_Streaks2.part
		LifeSpan	3
	End
End

Condition
	On 	Time
	Time 	4

	Event
		EName 	Flash
		Type	Local
		At	MidOffset
		Part	:Explosion\Explosion_Flash1.part
		Part	:Explosion\Explosion_Flash2.part
		Lifespan	2
	End

	Event
		EName 	Flames1
		Type	Local
		At	MidOffset
#		Part	:Explosion\Explosion_Smoke.part
		Part2	:Explosion\Explosion_Plasma.part
#		Part2	:Explosion\Explosion_Plasma2.part
		LifeSpan	2
	End

#	Event
#		EName 	Flames1
#		Type	Local
#		At	MidOffset
#		Part	:Explosion\Explosion_Streaks1.part
#		Part	:Explosion\Explosion_Streaks2.part
#		LifeSpan	2
#	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash
		Type	Local
		At	BaseOffset
		Part	:Explosion\Explosion_Flash1.part
		Part	:Explosion\Explosion_Flash2.part
		Lifespan	2
	End

	Event
		EName 	Flames1
		Type	Local
		At	BaseOffset
#		Part	:Explosion\Explosion_Smoke.part
		Part2	:Explosion\Explosion_Plasma.part
#		Part2	:Explosion\Explosion_Plasma2.part
		LifeSpan	2
	End

#	Event
#		EName 	Flames1
#		Type	Local
#		At	BaseOffset
#		Part	:Explosion\Explosion_Streaks1.part
#		Part	:Explosion\Explosion_Streaks2.part
#		LifeSpan	2
#	End
End

## NOVA #######################################################
 
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Shockwave
#		Type	Local
#		At	TopOffset
#		BhvrOverride
#			PyrRotateJitter	0 0 0
#		End
#		Lifespan	10
#	End
#
#	Event
#		EName 	Prime1
#		Type	Local 
#		At	TopOffset
#		bhvr	:Explosion\Explosion_ShockwaveScaler1.bhvr
#		Part	:Explosion\Explosion_ShockwaveTrails1.part
#		Part	:Explosion\Explosion_ShockwaveSmoke1.part
#		Part	:Explosion\Explosion_ShockwaveSmoke2.part
#		LifeSpan	15
#	End
#
#	Event
#		EName	ring
#		Type	start
#		At	TopOffset
#		bhvr	:Explosion\Explosion_ShockwaveScaler2.bhvr
#		geom	MegaRing2
#		Part	:Explosion\Explosion_Shockwave1.part
#		LifeSpan	15
#	End
#End

## SPARKS #######################################################

Condition
	On 	time
	Time 	0
	Repeat 	8

	Event 
		EName 	Sparks
		Type 	Start
		At 	TopOffset
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
		At 	TopOffset
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan	90
		LifespanJitter 	30
	End
End

## PYLON DEBRIS #######################################################

Condition
	On	Time
	Time 	5

	Event
		EName 	PylonDebris01
		Type	Start
		At	Root
		Bhvr	behaviors\GenericGeometryFade.bhvr
		BhvrOverride
			PyrRotateJitter	2.5 0 5
		End
		Geom	RiktiPylon_Debris01
		part	:Explosion\LingeringSmoke1.part
		part	:Explosion\LingeringSmoke2.part
		Lifespan	300
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	5

	Event
		EName 	PylonDebris03
		Type	Start
		At	PylonDebris01
		altPiv	3
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.03125 0.03125 0.03125
			Drag			0.005
			StartColor		255 255 255
		End
		Geom	RiktiPylon_Debris03
		Lifespan	150
		LifespanJitter	30
	End

	Event
		EName 	PylonDebris04
		Type	Start
		At	PylonDebris01
		altPiv	4
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
			Drag			0.005
			StartColor		255 255 255
		End
		Geom	RiktiPylon_Debris04
		Lifespan	150
		LifespanJitter	30
	End

	Event
		EName 	PylonDebris02
		Type	Start
		At	PylonDebris01
		altPiv	2
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0
			InitialVelocity		0.0 0.375 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
			Drag			0.005
			StartColor		255 255 255
		End
		Geom	RiktiPylon_Debris02
		Lifespan	150
		LifespanJitter	30
	End

	Event
		EName 	PylonDebris05
		Type	Start
		At	PylonDebris01
		altPiv	5
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0
			InitialVelocity		0.0 0.75 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
			Drag			0.005
			StartColor		255 255 255
		End
		Geom	RiktiPylon_Debris05
		Lifespan	150
		LifespanJitter	30
	End

	Event
		EName 	PylonDebris06
		Type	Start
		At	PylonDebris01
		altPiv	6
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0
			InitialVelocity		0.0 0.75 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.005
			StartColor		255 255 255
		End
		Geom	RiktiPylon_Debris06
		Lifespan	150
		LifespanJitter	30
	End

	Event
		EName 	PylonDebris07
		Type	Start
		At	PylonDebris01
		altPiv	7
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0
			InitialVelocity		0.0 0.75 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.005
			StartColor		255 255 255
		End
		Geom	RiktiPylon_Debris07
		Lifespan	150
		LifespanJitter	30
	End

	Event
		EName 	PylonDebris08
		Type	Start
		At	PylonDebris01
		altPiv	8
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0
			InitialVelocity		0.0 1.00 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.005
			StartColor		255 255 255
		End
		Geom	RiktiPylon_Debris08
		Lifespan	150
		LifespanJitter	30
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
			InitialVelocity		0.0 0 0.0
			InitialVelocityJitter	1.0 0.5 1.0
			Scale			0.05 0.05 0.05
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
	Repeat	3

	Event
		EName 	metal
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	2.0 0.5 2.0
			Scale			0.025 0.025 0.025
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