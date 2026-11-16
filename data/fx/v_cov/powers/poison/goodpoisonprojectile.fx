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
	Time 	0

	Event
		EName	Spike
		Type	Local
		At	head
		
		Geom	LocalPivotz
		bhvr	behaviors/scale1a.bhvr	
		SOund	poisonprojectile 80 30 .88
	End

	Event
		EName	Prime
		Type	start
		At	Spike
		Altpiv	1

		Bhvr	:YellowGlassScaleLarge.bhvr
		Geom	PoisonGlassBall

		Magnet	Target
		LookAt	Target
		
		Part	:QuillTrailBlack.part
		Part	:QuillTrail.part
		Part	:QuillTrailLight.part

		Part	:QuillTrailStreak.part
		Part2	:YellowSmallBallzTrail.part
		Part3	weaponFx/QuillBubblesTrail.part
		Part5	:QuillBallzTrail.part
		Part5	:QuillBallzTrailBlack.part

	End

	Event
		EName	Prime2
		Type	start
		At	Spike
		Altpiv	1
	
		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
	#		Part1	weaponFx/QuillTrail2.part
#		Part2	:PoisonHead.part
		
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
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
		

	End

End

End				