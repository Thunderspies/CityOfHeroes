#########################################################
##	Fire_Ball
##

#########################################################

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


#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Hand1
		Type	Local 
		At	WepR
		geom	staff_04
		
	End

End

Condition
	On 	Time
	Time 	48

	Event
		EName 	prime
		Type	Start 
		At	Hand1
		altpiv	1
		Bhvr	behaviors/CrystalStaffProjectile2.bhvr
		geom	ForceBoltShell
		
		Magnet	Target
		LookAt	Target
		
		Part1	:CrystalStaffHeadGlow.part
		Part2	:CrystalStaffBallz.part
		#Part3	:DarkEmberLarge.part
		Sound crystalstaff 80 80 .7
		
	End

	
End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	
End


End	


			