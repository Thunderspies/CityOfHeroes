
FxInfo

Flags    InheritAlpha InheritAnimScale

LifeSpan 100



##### first bot flyby  ################################

Condition
	On 	Time
	Time 	0


	Event
		EName	1st_bot
		Type	Local
		At	BendMystic
		Geom	GEO_Skiff_prop
		Bhvr	Behaviors/fadein.bhvr
		LifeSpan 26
	End

End


#######  1st explode  #####################################


Condition
	On 	Time
	Time 	26


	Event
		EName 	Fire
		Type	Start
		At	BendMystic	
		Part	Explosions\Large\SmokeExplosion.part
		Part1	Explosions\Large\FireExplosion2.part
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Start
		At	BendMystic
		Part1	Explosions\Large\GernadeLauncherSparks2.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	TankBlast_flash
		Type	Start
		At	BendMystic

		Part	Explosions\Large\FireExplosionFlash.part
		Lifespan 5
		
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	BendMystic
		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
		Part	Explosions\Large\FireBallHitRing2a.part
		Lifespan 8
	
	End


End


########  second bot flyby  ################################

Condition
	On 	Time
	Time 	32


	Event
		EName	2nd_bot
		Type	Local
		At	BendMystic
		Geom	GEO_Skiff_prop
		Bhvr	Behaviors/fadein.bhvr
		LifeSpan 26
	End

End


#######  2nd explode  #####################################


Condition
	On 	Time
	Time 	58


	Event
		EName 	Fire
		Type	Start
		At	BendMystic	
		Part	Explosions\Large\SmokeExplosion.part
		Part1	Explosions\Large\FireExplosion2.part
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Start
		At	BendMystic
		Part1	Explosions\Large\GernadeLauncherSparks2.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	TankBlast_flash
		Type	Start
		At	BendMystic

		Part	Explosions\Large\FireExplosionFlash.part
		Lifespan 5
		
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	BendMystic
		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
		Part	Explosions\Large\FireBallHitRing2a.part
		Lifespan 8
	
	End

#	Event	
#		ENAME	ThunderShake1
#		Type	Local
#		At	Origin
#		Bhvr	Behaviors/SSThunderClapShake.bhvr
#		Lifespan 8
#		
#		
#	End


End

######  Skiff shooting  ###########################################################


#### 1st Missle  ###################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Prime
		Type	start
		At	WepR
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missile
		Part1	AnimatedCharacterParts/5thmissile_smoke.part
		Part2	AnimatedCharacterParts/5thmissile_smoke2.part
		Part3	AnimatedCharacterParts/5thmissile_fire.part
		Magnet	BendMystic
		LookAt	BendMystic
		Lifespan 10
	End

	
	Event
		EName 	Blast1
		Type	start
		At	WepR
		Geom	Fx_GunBurst
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
		Lifespan 3
	End

	Event
		EName 	KickBackSmoke
		Type	start
		At	WepR
		Part1	WEAPONFX/RocketLauncherSmoke.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part4	WEAPONFX/Gunfire_MissleShell.Part
		Lifespan 7
	End


	Event
		EName 	TankBlast_flash
		Type	Local
		At	WepR

		Part	WEAPONFX/FireExplosionFlash.part
		#Part2	WEAPONFX/GernadeLauncherSpark.part
		Lifespan 5
		
	End


End


#### 2nd Missle  ###################

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime2
		Type	start
		At	WepR
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missile
		Part1	AnimatedCharacterParts/5thmissile_smoke.part
		Part2	AnimatedCharacterParts/5thmissile_smoke2.part
		Part3	AnimatedCharacterParts/5thmissile_fire.part
		Magnet	BendMystic
		LookAt	BendMystic
		Lifespan 10
	End

	
	Event
		EName 	Blast2
		Type	start
		At	WepR
		Geom	Fx_GunBurst
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
		Lifespan 3
	End

	Event
		EName 	KickBackSmoke2
		Type	start
		At	WepR
		Part1	WEAPONFX/RocketLauncherSmoke.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part4	WEAPONFX/Gunfire_MissleShell.Part
		Lifespan 7
	End



	Event
		EName 	TankBlast_flash2
		Type	Local
		At	WepR

		Part	WEAPONFX/FireExplosionFlash.part
		#Part2	WEAPONFX/GernadeLauncherSpark.part
		Lifespan 5
		
	End


End


End
