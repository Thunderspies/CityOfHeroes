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
	Time 	77


	Event
		EName 	tracer
		Type	start
		At	Prime
		Altpiv	1

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound Musket 100.0 100.0 .9
	End

End

##################################


Condition
	On 	Time
	Time	81

	Event
		Ename 	All
		Type	Destroy
	End
End


End				