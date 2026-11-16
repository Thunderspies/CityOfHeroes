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
	Inpname	Head
End


#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Spike
		Type	Local
		At	WepR
		
		Geom	PoisonGlassBall
		Bhvr	:YellowGlassScale.bhvr
		Sound Alkaloid1 80 80 .88
		LifeSpan	23
	End

End

Condition
	On 	Time
	Time 	34

	Event
		EName	Prime
		Type	start
		At	Spike

		Bhvr	:LobbYellowGlassScaleLarge.bhvr
		Geom	PoisonGlassBall

		Magnet	Target
		LookAt	Target
		
		Part	:QuillTrailBlack.part
		Part	:QuillTrail.part
		Part	:QuillTrailLight.part

		Part	:QuillTrailStreak.part
		Part2	:YellowSmallBallzTrail.part
		Part3	:QuillBubblesTrail.part
		Part3	:QuillBubblesTrail.part
		Part5	:QuillBallzTrail.part
		Part5	:QuillBallzTrailBlack.part
		Part	:YellowSmallStarsTrail.part

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