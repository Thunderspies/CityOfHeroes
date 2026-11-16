#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	T_Chest
End

Input  
	InpName	Mystic
End

Input  
	InpName	BendMystic
End

Input  
	InpName	WepL
End

Lifespan	110

##################################
Condition
	On 	Time
	Time 	0




	Event
		EName 	Prime
		Type	start
		At	WepL
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missile
		Part1	AnimatedCharacterParts/5thmissile_smoke.part
		Part2	AnimatedCharacterParts/5thmissile_smoke2.part
		Part3	AnimatedCharacterParts/5thmissile_fire.part
		#Part1	:MachineGunFlash.part
		#Part2	:MachineGunGlow.part
		Magnet	BendMystic
		LookAt	BendMystic
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
	End

	Event
		EName 	KickBackSmoke
		Type	start
		At	WepL
		Part1	:RocketLauncherSmoke.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part4	WEAPONFX/Gunfire_MissleShell.Part
	End

	Event
		EName 	TankBlast_blast
		Type	Local
		At	WepL
		Part1	:FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	TankBlast_flash
		Type	Local
		At	WepL

		Part	:FireExplosionFlash.part
		#Part2	:GernadeLauncherSpark.part
		Lifespan 5
		
	End


End


##################################

#Condition
#	On Primehit 
#	
#	Event
#		EName	Explosion
#		Type 	Local
#		At	T_Chest
#		Part1	:Shotgunflash.part
#		Part2	:ShotgunflashStar.part
#		Sound	explohit2 100.0 60.0 1.0
#	End		
#	
#	Event
#		Ename	Prime
#		Type	Destroy
#	End
#
#	Event
#		Ename	Explosion
#		Type	Destroy
#	End
#
#End

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
	Time	7

	Event
		Ename 	KickBackSmoke
		Type	Destroy
	End

End



End				