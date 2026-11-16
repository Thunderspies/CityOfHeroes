#########################################################
##	
##

FxInfo
LifeSpan  150

Input  
	InpName	Target
End

Input  
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	0

	
	Event
		EName 	Prime
		Type	local
		At	WepR
		Geom	LaserAssault
				
	End
End

Condition
	On 	Time
	Time 	46


	Event
		Type	start
		At	Prime
		Altpiv	1
		
		Geom	Fx_GunBurstHolyShotgun
		Bhvr	Behaviors/ShotGun.bhvr	
		Lifespan		2
	End

	Event
		Type	start
		At	Prime
		Altpiv	1

		Part	:EmchantedMusketMussleFlashStreaks.part
		Part	:EmchantedMusketMussleFlashStreaksBIg.part
		Part	:Gunfire_EmchantedMusket.part
		Lifespan		5
	End

End

Condition
	On 	Time
	Time 	47


	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	LaserAssault
		
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	1
		

		Part1	:EnchantedPistolMuzzflash.part
		Part2	:EnchantedPistolsmoke.part
		Part3	:EnchantedPistolMuzzflashSmall.part
		sound	lee1 100.0 70.0 .8
		
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
		Part4	WEAPONFX/Gunfire_PistolShell.Part
	End

End

##################################

Condition
	On 	Primehit
	
	Event
		Ename 	Prime
		Type	Destroy
	End
	
	Event
		Ename 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	49

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	57

	Event
		Ename 	tracer2
		Type	Destroy
	End

End


End				