#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End
##################################
Condition
	On	Time
	Time	0
		
	Event	
		ENAME	RightFoot
		Type	Posit
		At	FootR
		part1	:MartialArtsAreaCloud.part
		part2	:MartialArtsAreaCloudSteam.part
		part3	:MartialArtsAreaSteam.part
		#part4	:MartialArtsAreaMist.part
	End

	Event	
		ENAME	LeftFoot
		Type	Posit
		At	FootL
		part1	:MartialArtsAreaCloud.part
		part2	:MartialArtsAreaCloudSteam.part
		part3	:MartialArtsAreaSteam.part
		#part4	:MartialArtsAreaMist.part

	End
End

	
Condition
	On	Time
	Time	30
		
	Event	
		ENAME	RightFoot2
		Type	Posit
		At	FootR
		part1	:MartialArtsAreaCloud.part
		part2	:MartialArtsAreaCloudSteam.part
		part3	:MartialArtsAreaSteam.part
		#part4	:MartialArtsAreaMist.part
	End

	Event	
		ENAME	LeftFoot2
		Type	Posit
		At	FootL
		part1	:MartialArtsAreaCloud.part
		part2	:MartialArtsAreaCloudSteam.part
		part3	:MartialArtsAreaSteam.part
		#part4	:MartialArtsAreaMist.part
	End

End

Condition
	On	Time
	Time	25
		
	Event
		Ename	LeftFoot
		Type	Destroy
	End

	Event
		Ename	RightFoot
		Type	Destroy
	End

End

Condition
	On	Time
	Time	75
		
	Event
		Ename	LeftFoot2
		Type	Destroy
	End

	Event
		Ename	RightFoot2
		Type	Destroy
	End

End


End				