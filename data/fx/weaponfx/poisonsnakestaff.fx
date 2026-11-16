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
	Time 	0

#	Event
#		EName 	geomtemp
#		Type	Local 
#		At	WepR
#		geom	GEO_WepR_Staff_2
#		
#	End

	Event
		EName 	Hand1
		Type	Local 
		At	WepR
		geom	Staff_1
		
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName	Prime
		Type	start
		At	Hand1
		altpiv	1
		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrail.part
		Part2	:QuillTrailSpecks.part
		Part3	:QuillBubblesTrail.part
		Part5	:QuillBallzTrail.part
		Sound poisonstaff 70 70 .8
	End

	Event
		EName	Prime2
		Type	start
		At	Hand1
		altpiv	1
		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrail2.part
		Part2	:PoisonHead.part
		
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