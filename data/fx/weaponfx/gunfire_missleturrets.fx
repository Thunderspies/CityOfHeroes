#########################################################
##	
##

FxInfo

Lifespan	110

##################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	tracer
		Type	Start
		At	ToeR
		
		Geom	Fx_GunBurst
		Bhvr	:TurentBlastOrient.bhvr
		
		LifeSpan  1

	End


	Event
		EName 	Prime
		Type	start
		At	ToeR
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missile
		Part1	AnimatedCharacterParts/5thmissile_smoke.part
		Part2	AnimatedCharacterParts/5thmissile_smoke2.part
		Part3	AnimatedCharacterParts/5thmissile_fire.part
	
		Magnet	BendMystic
		LookAt	BendMystic
	End

	
	Event
		EName 	Blast1
		Type	start
		At	ToeR		
		Bhvr	:TurentBlastOrient1.bhvr
		Geom	Fx_GunBurst
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
	End

	Event
		EName 	KickBackSmoke
		Type	local
		At	ToeR
		Bhvr	:TurentBlastOrientEndSmoke.bhvr
		Part1	:TurrentEndSmoke.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part4	WEAPONFX/Gunfire_MissleShell.Part
	End

	Event
		EName 	TankBlast_blast
		Type	Local
		At	ToeR
		#Part1	:FireExplosion.part
		
		Lifespan 8
	End

	Event
		EName 	TankBlast_flash
		Type	Local
		At	ToeR

		#Part	WEAPONFX/FireExplosionFlash.part
		Lifespan 5
		
	End


End


##################################


Condition
	On 	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Explosion
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	3

	Event
		Ename 	Blast1
		Type	Destroy
	End

	
End

Condition
	On 	Time
	Time	15

	Event
		Ename 	KickBackSmoke
		Type	Destroy
	End

End



End				