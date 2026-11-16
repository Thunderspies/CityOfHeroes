#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	80

Input  
	InpName	Chest
End


###########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	geom
		Type	local
		At	Chest
		bhvr	Behaviors/SpinHit2.bhvr
		geom	CenterDummy

	End
	
	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	:WhiteEnergyBallsHit.part
		Part2	:WhiteDarkEnergyBallsHit.part
		Part3	:WhiteSolidEnergyBallsHit.part
		Part4	:WhiteBlastHitRings.part
		Sound legacytorrenthit2 90 90 .9
	End


	Event
		EName 	Burst
		Type	local
		At	Chest
		Part1	:WhiteEnergyBallsBurst.part
		Part2	:WhiteEnergyBallsBurst2.part
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName 	hit2
		Type	Destroy
				
	End


End	


End