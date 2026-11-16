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

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	TestSphere
#		Type	Local
#		At	Hips
#		Bhvr	:RiktiHeavyAssaultSuit\Explosion_Final_SphereScaler.bhvr
#		Geom	GEO_WhiteSphere
#		Lifespan	30
#	End
#End


## EXPLOSION #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash
		Type	Local
		At	Origin
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_Flash1.part
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_Flash2.part
		Lifespan	10
	End

	Event
		EName 	Flames1
		Type	Local
		At	Origin
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_Smoke.part
		Part2	:RiktiHeavyAssaultSuit\Explosion_Final_Plasma.part
		Part2	:RiktiHeavyAssaultSuit\Explosion_Final_Plasma2.part
		LifeSpan	4
	End

	Event
		EName 	Flames1
		Type	Local
		At	Origin
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_Streaks1.part
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_Streaks2.part
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
		At	Origin
		BhvrOverride
			PyrRotateJitter	15 0 15
		End
		Lifespan	10
	End

	Event
		EName 	Prime1
		Type	Local 
		At	Shockwave
		bhvr	:RiktiHeavyAssaultSuit\Explosion_Final_ShockwaveScaler1.bhvr
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_ShockwaveTrails1.part
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_ShockwaveSmoke1.part
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_ShockwaveSmoke2.part
		LifeSpan	15
	End

	Event
		EName	ring
		Type	start
		At	Shockwave
		bhvr	:RiktiHeavyAssaultSuit\Explosion_Final_ShockwaveScaler2.bhvr
		geom	MegaRing2
		Part	:RiktiHeavyAssaultSuit\Explosion_Final_Shockwave1.part
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
		At 	Origin
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
		At 	Origin
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan	90
		LifespanJitter 	30
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
		ChildFX Explosions\VehicleExplosion\RiktiHeavyAssaultSuit\Explosion_Secondary_MetalDebris.fx
	End
End

## GEO DEBRIS PIECES #######################################################

Condition
	On	Time
	Time 	0

	Event
		Ename	RHAS_AltPivots
		Type	Local
		At	Hips
		Geom	RiktiHeavyAssuiltSuit_AltPivots
	End

	Event
		Ename	DebrisPiece1
		Type	Start
		At	RHAS_AltPivots
		AltPiv	1
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			PyrRotateJitter		0 0 0
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
			InitialVelocity		0.0 1.00 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Drag			0.01
			physSFriction 	 	0.6
			StartColor		255 255 255
		End
		Geom	rikti_heavy_assault_debris_01
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	DebrisPiece2
		Type	Start
		At	RHAS_AltPivots
		AltPiv	2
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			PyrRotateJitter		0 0 0
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
			InitialVelocity		0.0 0.75 0.0
			InitialVelocityJitter	0.25 0.125 0.25
			Drag			0.01
			physSFriction 	 	0.6
			StartColor		255 255 255
		End
		Geom	rikti_heavy_assault_debris_02
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	DebrisPiece3
		Type	Start
		At	RHAS_AltPivots
		AltPiv	3
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			PyrRotateJitter		0 0 0
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.0625 0.0625 0.0625
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.375 0.125 0.375
			Drag			0.01
			physSFriction 	 	0.6
			StartColor		255 255 255
		End
		Geom	rikti_heavy_assault_debris_03
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	DebrisPiece4
		Type	Start
		At	RHAS_AltPivots
		AltPiv	4
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			PyrRotateJitter		0 0 0
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.375 0.125 0.375
			Drag			0.01
			physSFriction 	 	0.6
			StartColor		255 255 255
		End
		Geom	rikti_heavy_assault_debris_04
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	DebrisPiece5
		Type	Start
		At	RHAS_AltPivots
		AltPiv	5
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			PyrRotateJitter		0 0 0
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.375 0.25 0.375
			Drag			0.01
			physSFriction 	 	0.6
			StartColor		255 255 255
		End
		Geom	rikti_heavy_assault_debris_05
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	DebrisPiece6
		Type	Start
		At	RHAS_AltPivots
		AltPiv	6
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			PyrRotateJitter		0 0 0
			Scale			1.0 1.0 1.0
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			InitialVelocity		0.0 0.125 0.0
			InitialVelocityJitter	0.375 0.25 0.375
			Drag			0.01
			physSFriction 	 	0.6
			StartColor		255 255 255
		End
		Geom	rikti_heavy_assault_debris_06
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	30
	End

End

#######################################################################

End			