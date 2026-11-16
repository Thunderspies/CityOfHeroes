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
		
		Part	:coneQuillTrailBlack.part
		Part	:coneQuillTrail.part
		Part	:coneQuillTrailLight.part

		Part	:coneQuillTrailStreak.part
		Part2	:coneYellowSmallBallzTrail.part
		#Part3	weaponFx/QuillBubblesTrail.part
		Part5	:coneQuillBallzTrail.part
		Part5	:coneQuillBallzTrailBlack.part

	End

	Event
		EName	Prime2
		Type	start
		At	Spike
		Altpiv	1
	
		Bhvr	behaviors/Quillprojectile1.bhvr
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
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
		

	End

End

End				