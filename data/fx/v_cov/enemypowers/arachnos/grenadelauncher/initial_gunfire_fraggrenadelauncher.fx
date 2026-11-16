#########################################################
##	
##

FxInfo
LifeSpan  200

Input  
	InpName	Origin
End

Input  
	InpName	T_Chest
End

Input  
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	40


	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	LaserAssault
		
	End

	Event
		EName 	tracer
		Type	Start
		At	Blast
		Altpiv	1
		Geom	Fx_GunBurst
		Bhvr	Behaviors/ShotGun.bhvr
		LifeSpan  3
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	1
		
		
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		sound	shotgun5 120.0 30.0 .9
		
	End
		
	Event
		EName 	Prime
		Type	start 
		At	Blast
		Altpiv	1
		geom	grenade
		bhvr	behaviors/MorterLobbprojectile2.bhvr
		Part1	WeaponFx/BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

End

Condition
	On 	Time
	Time 	55


	Event
		EName 	Blast
		Type	local
		At	Blast
		Altpiv	2
		#Part1	WEAPONFX/Gunfire_ShotgunShell.Part
		ChildFX	V_COV\PhysicsEnabled\ShotGunShellEject.fx
	End

End

##################################

Condition
	On 	PrimeHit
	
	Event
		Type	start 
		At	T_Root
		ChildFx	V_COV\EnemyPowers\Arachnos\GrenadeLauncher\FragGrenadeHit.fx
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

	Event
		Ename 	Trail
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	42

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	50

	Event
		Ename 	tracer2
		Type	Destroy
	End

End


End				