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
	Time 	30


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
	Time 	45


	Event
		EName 	Blast
		Type	local
		At	Blast
		Altpiv	2
		Part1	WEAPONFX/Gunfire_ShotgunShell.Part
	End

End

##################################

Condition
	On 	PrimeHit
	
	
	Event
		EName   RadiusFxScale
		Type	local
		At	T_Root
		Bhvr	powers/firecontrol/Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	start 
		At	RadiusFxScale
		bhvr    WEAPONS/Bow/TrickArrow/Infernobubble.bhvr
		Part	WEAPONS/Bow/TrickArrow/PoisonYellow2.part
		Part	WEAPONS/Bow/TrickArrow/PoisonGlows.part
		
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	32

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	40

	Event
		Ename 	tracer2
		Type	Destroy
	End

End


End				