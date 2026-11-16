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
		Part1	:StrengthStreak.part
		#Part2	:StrengthPulse.part
		Part3	:StrengthGlow.part
		Sound handclap 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Part1	:StrengthStreak.part
		#Part2	:StrengthPulse.part
		Part3	:StrengthGlow.part
	End
End


Condition
	On 	Time
	Time 	30

	Event
		EName 	Mallet1
		Type	start 
		At	mystic
		Geom	SuperStrengthBubble
		bhvr	behaviors/ScaleBubble.bhvr
		
	End

	Event
		EName 	Mallet2
		Type	start 
		At	mystic
		
		Part1	:SuperStrengthHitEmber.part
		Part2	:SuperStrengthHitStar.part
		Part3	:SuperStrengthHitRing.part
		Part4	:SuperStrengthHitRing2.part
		Part5	:SuperStrengthHitGlows2.part
		Sound PunchHit2 100.0 100.0 .8
	End

End


Condition
	On 	Time
	Time 	37

	Event
		EName	Mallet1
		Type	Destroy
		
	End
	
	Event
		EName	Mallet2
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

