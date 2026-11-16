#########################################################
## Rikti Heavy Assault Suit - Explosion/Death FX
#########################################################

FxInfo  

Flags InheritAnimScale

Lifespan 900

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Chest
		Sound flierexplo1 200 200 .93
	End
End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	BaseOffset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 4 0
			Scale		1 1 1
		End
	End

	Event
		EName	Splat
		Type	start
		At	Origin
		Bhvr	:Explosion_Splat1.bhvr
		Splat	ScorchMark.tga #Explosion_Crater1.tga #StoneCracks.tga
		Lifespan	900
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
			Shake          0.5	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.02	#how long shake lasts
			ShakeRadius    512	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
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
		Bhvr	ENEMYFX\Rikti\UXB\Explosion\Explosion_SphereScaler.bhvr
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
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_Flash1.part
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_Flash2.part
		Lifespan	10
	End

	Event
		EName 	Flames1
		Type	Local
		At	BaseOffset
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_Smoke.part
		Part2	ENEMYFX\Rikti\UXB\Explosion\Explosion_Plasma.part
		Part2	ENEMYFX\Rikti\UXB\Explosion\Explosion_Plasma2.part
		LifeSpan	4
	End

	Event
		EName 	Flames1
		Type	Local
		At	BaseOffset
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_Streaks1.part
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_Streaks2.part
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
		bhvr	ENEMYFX\Rikti\UXB\Explosion\Explosion_ShockwaveScaler1.bhvr
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_ShockwaveTrails1.part
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_ShockwaveSmoke1.part
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_ShockwaveSmoke2.part
		LifeSpan	15
	End

	Event
		EName	ring
		Type	Start
		At	BaseOffset
		bhvr	ENEMYFX\Rikti\UXB\Explosion\Explosion_ShockwaveScaler2.bhvr
		geom	MegaRing2
		Part	ENEMYFX\Rikti\UXB\Explosion\Explosion_Shockwave1.part
		LifeSpan	15
	End
End

#######################################################################

End			