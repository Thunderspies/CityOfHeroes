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
		At	Origin
		bhvr	Behaviors/SpinHit2.bhvr
#		geom	CenterDummy

	End
	
	Event
		EName 	hit2
		Type	local
		At	Origin

		Part	POWERS\EnergyBlast\EnergyBallsHitBlack.part
		Part3	POWERS\EnergyBlast\SolidEnergyBallsHitRed.part
		Part4	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Burst_RangedMissRed.part
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