#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	weporigin
		Type	local
		At	C_Head
		BhvrOverride
			PositionOffset	0 -.25 -2.2
			PyrRotate	0 90 0
		End
	End

	Event
		EName 	azimuth
		Type	StartPositOnly
		At	T_Root
		BhvrOverride
			PositionOffset	0 20 0
		End
	End

	Event
		EName 	second
		Type	local
		At	azimuth
		BhvrOverride
			PositionOffset	0 5 0
		End
	End
End

## MUZZLEFLASH ###########################################################

Condition
	On 	Time
	Time 	31

	Event
		EName 	Blast
		Type	Start
		At	weporigin
		Geom	Fx_GunBurst
		Sound mortarlaunch1 100.0 100.0 1.0
		Lifespan 3
	End

	Event
		EName 	tracer
		Type	start
		At	weporigin
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Lifespan 12
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	31

	Event
		EName 	Prime
		Type	start
		At	weporigin
		Bhvr	behaviors\MorterLobbprojectile2.bhvr
		BhvrOverride
			InitialVelocity		0 2 0
			Drag			.01
			Gravity			.015
			TrackRate		1.8
			Scale			.5 .5 .5
			ScaleRate		1 1 1
			EndScale		.5 .5 .5
		End
		Part1	WEAPONFX\BeanBagDust2.part
		Part	:ChemTrailMist.part
		Part	:ChemTrailDrip.part
		geom	JunkBarrel_55gal_02
		Magnet	azimuth
		LookAt	azimuth
	End
End

## PRIME HIT ###########################################################

Condition
	On 	PrimeHit

	Event
		EName 	Smoke
		Type	StartPositOnly
		At	Azimuth
		Part1	:ChemExplodeSmoke1.part
		Part	:ChemSpray.part
		Part	:ChemMist.part
		Part	:NovaTendrils.part
		Lifespan 6
	End

	Event
		EName 	Sparks
		Type	StartPositOnly
		At	Azimuth
		Part	WEAPONFX\FireExplosionFlash.part
		#Part2	V_COV\WEAPONFX\DynamiteExplodeSparks.part
		Sound fuelairbomb 100.0 100.0 1.0
		Lifespan 5
	End

	Event
		Type	start
		At	azimuth
		ChildFX	:Heavy_FuelAirBomb_Child.fx
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End