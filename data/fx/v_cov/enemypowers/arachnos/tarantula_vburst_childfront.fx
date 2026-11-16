
FxInfo
	
LifeSpan	85



################################################################################
############################   c e n t e r   ###################################
################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterPos
		Type	Local
		At	Belt
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstCharge.bhvr
##		Geom	Testing_Target
	End

	Event
		EName	ChargeShot
		Type	Local
		At	EmitterPos

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstChargeStart.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargeFront.part
		PMagnet	EmitterPos
		Lifespan 8
	End
End