#########################################################
##	
##

#########################################################
##	
##

FxInfo

LifeSpan  100

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
	Time 	29

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	DartGun
	End

	Event
		EName 	Prime
		Type	start 
		At	Mystic
	
		Magnet	Target
		LookAt	Target
		Sound DartGun1 100.0 100.0 .5
		
	End


End

Condition
	On 	Time
	Time 	30

	Event
		EName 	tracer
		Type	start
		At	p2

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
	End

End

##################################


Condition
	On 	Time
	Time	33

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End
End

End

		