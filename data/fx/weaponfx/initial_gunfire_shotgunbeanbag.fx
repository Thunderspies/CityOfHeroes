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
		
		geom	BeanBag
		bhvr	Behaviors/BeanBagProjectile.bhvr
		Part	:BeanBagDust.part
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName 	trail
		Type	local
		At	Prime
		
		#bhvr	Behaviors/BeanBagProjectile.bhvr
		Part	:BeanBagDust.part
		Magnet	T_Chest
		LookAt  T_Chest
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
		Ename 	Prime
		Type	Destroy
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