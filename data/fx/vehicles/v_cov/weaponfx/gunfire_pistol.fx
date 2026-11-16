#########################################################
##	
##

FxInfo

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
	Time 	30

	Event
		EName 	point
		Type	local
		At	wepR

		Geom	Arachnos_Pistol02
	End
	
	Event
		EName 	tracer
		Type	start
		At	point
		altpiv	1
		
		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound heavypistol1 100.0 100.0 .9
	End

End

##################################


Condition
	On 	Time
	Time	35

	Event
		Ename 	All
		Type	Destroy
	End
End


End				