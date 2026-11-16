#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	origin
End

Input  
	Inpname	target
End

Input  
	Inpname	chest
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	Root
End

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UARMR
		part1	:StrengthBodyGlows.part
		PMagnet	LarmR
	End
	
	Event
		EName	Streak1
		Type	local
		At	UARML
		part1	:StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		part1	:StrengthBodyGlows.part
		PMagnet	WepR
	End
	
	Event
		EName	Streak1
		Type	local
		At	LarmL
		part1	:StrengthBodyGlows.part
		PMagnet	WepL
	End


	Event
		EName	Streak1
		Type	local
		At	WepR
		part1	:StrengthStreak.part
		part2	:StrengthPulse.part
		part3	:StrengthGlowToss.part
		Sound PunchA 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		part1	:StrengthStreak.part
		part2	:StrengthPulse.part
		part3	:StrengthGlowToss.part
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	concrete
		Type	local
		At	wepR
		geom	toss
		bhvr	behaviors/projectileSuperStrength.bhvr
			

	End

	Event
		EName	concrete1
		Type	start
		At	Root
		bhvr	behaviors/SlightQuake.bhvr
		Part1	:SuperStrengthCrack01.part		

	End

End

Condition
	On 	Time
	Time 	85

	Event
		EName	Streak1
		Type	local
		At	WepL
		part1	:StrengthBodyGlows1.part
		part2	:SuperStrengthStar.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		part1	:StrengthBodyGlows.part
		part2	:SuperStrengthStar.part
	End

End

Condition
	On 	Time
	Time 	90

	Event
		EName	concrete
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	90

	Event

		EName 	Prime
		Type	Start 
		At	WepR
		
		geom	toss
		Bhvr	behaviors/SuperStrengthprojectile.bhvr
		Magnet	Target
		LookAt	Target
	
		
	End

End


Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		

	End
	

End

Condition
	On 	Time
	Time 	100

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

