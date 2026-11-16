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
		geom	WindGround
		bhvr	behaviors/LevitateScale2.bhvr
		Part1	:AirGroundRocks.part
		#Sound	EnergyGlow1_loop 70 60 .7
	End
	
	Event	
		ENAME	poo2
		Type	Local #start
		At	Root
		part1	:GroundSplash.part
		part2	:GroundSteam.part
		#Sound	EnergyGlow2_loop 70 60 .9
	End

End
	
Condition
	On	Time
	Time 	80

	
	Event	
		ENAME	steamup
		Type	Local #start
		At	Root
		part1	:GroundSplash2.part
		part2	:GroundSteam2.part
	End


End


End				