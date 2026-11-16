#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Hips
End

LifeSpan	200

##################################
Condition
	On	Time
	Time	20
	
	Event
		EName   Hit
		Type	Local
		At	Hips
		part1	:DisruptionAura02.part
		#part2	:CrarzyEnergyBallMotion.part
	End
End

Condition
	On	Time
	Time	60

	Event
		EName   Hitballs
		Type	Local
		At	Hips
		#part1	:DisruptionAura02.part
		part2	:CrarzyEnergyBallMotion.part
	
	End
End


Condition
	On	Time
	Time	140
	
	Event
		Type	Destroy
		EName   Hitballs
	
	End
End

Condition
	On	Time
	Time	140
	
	Event
		Type	Destroy
		EName   Hit
	
	End

End

End				