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
	Time 	46

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
		EName 	tracer
		Type	start
		At	Prime
		Altpiv	1

		Part1	:EnchantedPistolMuzzflash.part
		Part2	:EnchantedPistolsmoke.part
		Part3	:EnchantedPistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound Musket 100.0 100.0 .9
	End

End


Condition
	On 	Time
	Time 	55


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
	Time	51

	Event
		Ename 	All
		Type	Destroy
	End
End


End				