#########################################################
##	cot_waterfall_med.fx
##
FxInfo

Input  
	InpName	Origin
End


Input  
	InpName	Target
End

Input  
	InpName	WepR
End

LifeSpan	200

###########################################################


Condition
	On	Time
	Time	22
	

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	Fx_Spike01
		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrail.part
		Part2	:QuillTrailSpecks.part
		Part3	:QuillBubblesTrail.part
		Part4	:QuillTrailStreak.part
		Part5	:QuillBallzTrail.part
		Sound spewhiss2 90 90 .75
	End

	Event
		EName	Prime2
		Type	start
		At	WepR
		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrail2.part
			#Part2	:QuillTrailSpecks.part
			#Part3	:QuillBubblesTrail.part
			#Part4	:QuillTrailStreak.part
			#Part5	:QuillBallzTrail.part
	End

	Event
		EName	Prime
		Type	start
		At	WepR
		
		Part2	:QuillFillBurst.part
		Part3	:QuillFillBurst2.part
		Part4	:QuillBubblesBurst.part

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