#########################################################
##	
##

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
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
		EName 	Prime
		Type	local
		At	WepR
		Geom	Pistol
				
	End
End	

Condition
	On 	Time
	Time 	70

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
	Time 	71

	Event
		EName 	tracer
		Type	start
		At	Prime
		Altpiv	1

		Part1	:EnchantedPistolMuzzflash.part
		Part2	:EnchantedPistolsmoke.part
		Part3	:EnchantedPistolMuzzflashSmall.part
		Sound Musket 100.0 100.0 .9
	End

End

Condition
	On 	Time
	Time 	79


	Event
		EName 	Blast
		Type	local
		At	Prime
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
	Time	81

	Event
		Ename 	All
		Type	Destroy
	End
End


End				