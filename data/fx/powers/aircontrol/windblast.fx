#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Input  
	Inpname	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:AirHands.part
		Part2	:AirHandsmiddle.part
	End

	Event
		EName	InitialSwordFire2
		Type	Local 
		At	WepL
		Part1	:AirHands.part
		Part2	:AirHandsmiddle.part
	End

End


Condition
	On 	Time
	Time 	45

	Event
		EName	InitialSwordFire
		Type	Destroy
		
	
	End

	Event
		EName	InitialSwordFire2
		Type	Destroy
		
	
	End

End

################################################################


Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	Root
		Bhvr	behaviors/WindProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Geom	AirBlastWave
		part1	:WindBlast1.part
		part3	:WindBlast3.part
		
	End



End


Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End
		
	
End

End	


			