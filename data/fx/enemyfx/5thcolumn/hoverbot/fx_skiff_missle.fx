
FxInfo

Flags    InheritAlpha

LifeSpan 100



######  1st Missle  ####

Condition
	On 	Time
	Time 	0

	Event
		EName 	FrozenNode_1
		Type	start
		At	BendMystic
		Geom	Fx_GunBurst
		Lifespan 30
	End


	Event
		EName 	Prime
		Type	start
		At	WepL
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missile
		Part1	AnimatedCharacterParts/5thmissile_smoke.part
##		Part2	AnimatedCharacterParts/5thmissile_smoke2.part
##		Part3	AnimatedCharacterParts/5thmissile_fire.part
		Magnet	FrozenNode_1
		LookAt	FrozenNode_1
		Lifespan 20
	End

	
	Event
		EName 	Blast1
		Type	start
		At	WepL
		Geom	Fx_GunBurst
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
		Lifespan 3
	End

#	Event
#		EName 	KickBackSmoke
#		Type	start
#		At	WepL
#		Part1	WEAPONFX/RocketLauncherSmoke.part
#		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
#		Part4	WEAPONFX/Gunfire_MissleShell.Part
#		Lifespan 7
#	End

#	Event
#		EName 	TankBlast_blast
#		Type	Local
#		At	WepL
#		Part1	WEAPONFX/FireExplosion.part
#		Lifespan 8
#	End

#	Event
#		EName 	TankBlast_flash
#		Type	Local
#		At	WepL
#
#		Part	WEAPONFX/FireExplosionFlash.part
#		#Part2	WEAPONFX/GernadeLauncherSpark.part
#		Lifespan 5
#		
#	End


End


#######  1st explode  #####################################


Condition
	On 	PrimeHit


	Event
		EName 	Fire
		Type	Start
		At	FrozenNode_1	
		Part	Explosions\Large\SmokeExplosion.part
		Part1	Explosions\Large\FireExplosion2.part
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Start
		At	FrozenNode_1
		Part1	Explosions\Large\GernadeLauncherSparks2.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	TankBlast_flash
		Type	Start
		At	FrozenNode_1

		Part	Explosions\Large\FireExplosionFlash.part
		Lifespan 5
		
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	FrozenNode_1
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




End