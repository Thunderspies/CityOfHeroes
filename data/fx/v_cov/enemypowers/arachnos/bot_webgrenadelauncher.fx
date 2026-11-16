#########################################################
##	
##

FxInfo
LifeSpan  100
Flags	InheritAnimScale

Input  
	InpName	Origin
End

Input  
	InpName	T_Chest
End

Input  
	InpName	C_Fore_ToeL
End


##################################
Condition
	On 	Time
	Time 	25


	Event
		EName 	Blast
		Type	local
		At	hips
#		geom	Testing_Target
		bhvr	:Bot_grenade_Offset.bhvr
	End



	Event
		EName 	tracer
		Type	Start
		At	Blast
#		Altpiv	1
#		Geom	Fx_GunBurst
#		Bhvr	Behaviors/ShotGun.bhvr
		LifeSpan  3
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		#Altpiv	1
		
		
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		sound	grenade2 120.0 30.0 .9
		
	End
		
	Event
		EName 	Prime
		Type	start 
		At	Blast
		#Altpiv	1
		geom	grenade
		bhvr	behaviors/MorterLobbprojectile2.bhvr
		Part1	WeaponFx/BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

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
		bhvr    WEAPONs/Bow/TrickArrow/stickyBubble.bhvr
		Part	WEAPONs/Bow/TrickArrow/StickyYellow2.part
		Part	WEAPONs/Bow/TrickArrow/StickyGlows.part
		
		LifeSpan	30
		
	End		

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