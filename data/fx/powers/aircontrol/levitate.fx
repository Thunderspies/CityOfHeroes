#########################################################
##	chill touch hit
FxInfo


Input  
	InpName	Root
End

LifeSpan	100

##################################

Condition
	On	Time
	Time	0

	Event	
		ENAME	poo
		Type	Local #start
		At	Root
		part1	:LevitateMistBright2.part
		#part2	:LevitateMistBright.part
		part3	:EnsnareRing1.part
		geom	AirFunnel03
		bhvr	behaviors/LevitateScale.bhvr
		PMAGNET	Head
	End
	
	
End


Condition
	On	Time
	Time	15
	
	Event	
		Type	local
		At	poo
		altpiv	1
		part1	:LevitateSplash.part
		part2	:LevitateSteam.part
		part3	:LevitateRocks.part
		
	End

End


End				