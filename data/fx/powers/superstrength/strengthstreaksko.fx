#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Mystic
End

Input  
	Inpname	Chest
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
	InpName	Col_L
End

Input  
	InpName	Col_R
End

######################################################################

Condition
	On 	Time
	Time 	0


	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	:StrengthStreak.part
		
		Sound PunchBB 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthStreak.part
		
	End

	Event
		EName	Streak1
		Type	local
		At	Chest
		#Part1	:StrengthBodyGlows1.part
		
	End

	Event
		EName	Streak1
		Type	local
		At	Col_R
		#Part1	:StrengthBodyGlows.part
		PMagnet	UarmR
	End
	
	Event
		EName	Streak1
		Type	local
		At	Col_R
		#Part1	:StrengthBodyGlows.part
		PMagnet	UarmL
	End

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Part1	:StrengthBodyGlows.part
		PMagnet	LarmR
	End
	
	Event
		EName	Streak1
		Type	local
		At	UARML
		Part1	:StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Part1	:StrengthBodyGlows.part
		PMagnet	WepR
	End
	
	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part1	:StrengthBodyGlows.part
		PMagnet	WepL
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		Part1	:StrengthBodyGlowsSmall.part
		
	End
	
	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthBodyGlowsSmall.part
		
	End
	
End

Condition
	On 	Time
	Time 	40

	Event
		EName	Streak1
		Type	local
		At	WepR
		
		Part2	:StrengthPulseKO.part
		Part3	:StrengthGlowKO.part
		
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		
		Part2	:StrengthPulseKO.part
		Part3	:StrengthGlowKO.part
	End
End


Condition
	On 	Time
	Time 	50

	Event
		EName	Streak1
		Type	local
		At	WepR
		
		Part2	:StrengthRingKO.part
		
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		
		Part2	:StrengthRingKO.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Target
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		500
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End


Condition
	On 	Time
	Time 	90

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

