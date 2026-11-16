#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Hips
End

Lifespan	200

########################################################################


Condition
	On 	Time
	Time 	19

	Event
		EName	roothookup
		Type	local
		At	root
		Bhvr	behaviors/alpha.bhvr
		Geom	RootToChestAdjustment


	End

	Event
		EName	hookup
		Type	local
		At	roothookup
		altpiv  1
		Geom	LungeNode


	End


	Event
		EName	Prime
		Type	start
		At	hookup
		altpiv	2
		Geom	GEO_WepR_Shuriken

		Bhvr	:Shuriken.bhvr
		Part1	:ShurikenStreaks01.part

		Magnet	Target
		LookAt	Target


	End

End


Condition
	On 	Time
	Time 	14.5

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound clawtwirl5 100.0 100.0 .7
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

