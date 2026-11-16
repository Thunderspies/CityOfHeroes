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
	InpName	WepR
End
##################################
Condition
	On 	Time
	Time 	0


	Event
		EName 	Blast1
		Type	start
		At	WepR
		Geom	Fx_GunBurst
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound grexplo2 120.0 120.0 1.0
	End

	Event
		EName 	KickBackSmoke
		Type	start
		At	WepR
		Part1	:RocketLauncherSmoke.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part4	WEAPONFX/Gunfire_MissleShell.Part
	End

	Event
		EName 	TankBlast_blast
		Type	Local
		At	WepR
		Part1	:FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	TankBlast_flash
		Type	Local
		At	WepR

		Part	:FireExplosionFlash.part
		#Part2	:GernadeLauncherSpark.part
		Lifespan 5
		
	End


	Event
		At	Origin
		Type	local
		Bhvr	Vehicles/tank/tank_tremor.bhvr
		Lifespan 30
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

#	Event
#		EName 	Fire
#		Type	Posit
#		At	T_Chest
#		Part1	:FireExplosion.part
#		#Part2	powers/firecontrol/FireWeaponHitSpark.part
#		Lifespan 8
#	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part1	:GernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

#	Event
#		EName 	Sparks
#		Type	Posit
#		At	T_Chest
#		Part	:FireExplosionFlash.part
#		#Part2	:GernadeLauncherSpark.part
#		Lifespan 5
#		
#	End
#
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