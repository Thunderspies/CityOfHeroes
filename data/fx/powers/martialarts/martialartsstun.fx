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

Input  
	InpName	Chest
End

##################################
Condition
	On	Time
	Time	0
		
	Event	
		ENAME	RightFoot
		Type	local
		At	FootR
		part1	:MartialArtsPowerCloud.part
		part2	:MartialArtsPowerCloudSteam.part
		part3	:MartialArtsPowerSteam.part
		
	End

	Event	
		ENAME	RightFootRing
		Type	local
		At	FootR
		part1	:MartialArtsPowerRing.part
			
	End
End

Condition
	On	Time
	Time	0
		
	Event	
		ENAME	RightFootstreaks1
		Type	local
		At	Chest
		part1	:MartialArtsPowerImpactRays.part
		Part2	:MartialArtsPowerImpactRing.part	
	End
	
	
	Event
		Ename	RightFootstreaks
		Type	Destroy
	End

End

Condition
	On	Time
	Time	34

	Event	
		ENAME	RightFootBall
		Type	local
		At	FootR
		part1	:MartialArtsPowerImpactBall.part
		part2	:MartialArtsPowerImpactRaysSmall.part	
	End

End

Condition
	On	Time
	Time	50
		
	Event	
		ENAME	RightFoot2
		Type	local
		At	FootR
		part1	:MartialArtsPowerCloud.part
		part2	:MartialArtsPowerCloudSteam.part
		part3	:MartialArtsPowerSteam.part
		
	End

End


Condition
	On	Time
	Time	34
		
	Event
		Ename	RightFoot
		Type	Destroy
	End

	Event
		Ename	RightFootRing
		Type	Destroy
	End

End





Condition
	On	Time
	Time	63
		
	Event
		Ename	LeftFoot
		Type	Destroy
	End

	Event
		Ename	RightFoot2
		Type	Destroy
	End

End

Condition
	On	Time
	Time	20
		
	Event
		Ename	RightFootstreaks1
		Type	Destroy
	End

End

Condition
	On	Time
	Time	45

	Event
		Ename	RightFootBall
		Type	Destroy
	End

End


End				