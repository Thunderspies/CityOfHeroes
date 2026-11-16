#########################################################
##	IceBolt
############################
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

LifeSpan	100

##################################
Condition
	On 	Time
	Time 	0



	Event
		EName 	Prime
		Type	Start 
		At	Origin
		Bhvr	behaviors/powers/coldcontrol/icebolt.bhvr
		
		Geom	FX_IceBolt
		Magnet	Target
		LookAt	Target
		Part1	powers/coldcontrol/icebolttail1.part
		Part2	powers/coldcontrol/icebolttail2.part
		Part3	powers/coldcontrol/IceBoltSnowtrail.part
	End


End

######################################
Condition
	On 	Time
	Time	100
	
	Event
		Ename 	SourceBlast
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	60
	
	Event
		Ename 	targetchill
		Type	Destroy
	End
End

End				