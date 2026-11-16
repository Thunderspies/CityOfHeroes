#########################################################
##	
##

FxInfo
LifeSpan  250

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

Flags    InheritAlpha


##################################
Condition
	On 	Time
	Time 	39

	
	Event
		EName 	P
		Type	start 
		At	WepR
		geom	Pistol
		Bhvr	Behaviors\Fasterprojectile.bhvr
		Part1	:EnergyPistolBullet.part
		Magnet	Target
		LookAt	Target
		
	End
	
	Event
		EName 	tracer
		Type	start
		At	P
		Altpiv	1
		Part1	WEAPONFX/Energy_pistolMuzzflash.part
		
	End

End

##################################


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
	Time	250

	Event
		Ename 	P
		Type	Destroy
	End


End

End				