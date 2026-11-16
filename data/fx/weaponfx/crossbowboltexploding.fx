#########################################################
##	
##

FxInfo

LifeSpan  190

##################################
Condition
	On 	Time
	Time 	45

	
	Event
		EName 	x
		Type	Local
		At	WepR
		Geom	Pistol
				
	End
	
	Event
		EName 	Prime
		Type	start
		At	x
		 Altpiv	1
		Bhvr	Behaviors\projectile2.bhvr
		Geom	CrossBowBolt
		Sound Crossbow1 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Chest
		Part1	WeaponFX/FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part1	WeaponFX/GernadeLauncherSparks.part
		#Part2	WeaponFX/GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part	WeaponFX/FireExplosionFlash.part
		#Part2	WeaponFX/GernadeLauncherSpark.part
		Lifespan 5
		
	End

End
				

Condition
	On 	Time
	Time 	17.5
	
	Event
		EName	streak
		Type	Destroy
		
	End
	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Prime1Hit
	
	Event
		EName 	Prime1
		Type	Destroy

	End


End

End
##################################