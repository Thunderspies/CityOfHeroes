
FxInfo
	
#LifeSpan	85



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
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBoltEmitter.bhvr
##		Geom	Testing_Target
	End

	Event
		EName	Emitter
		Type	Local
		At	Hair

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstChildEmit.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On	Time
	Time	1

	Event
		EName	Charge
		Type	Local
		At	Emitter

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCharge.part
		PMagnet	EmitterPos
		#Lifespan 25
	End
End

################################################################################
##############################    L e f t    ###################################
################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterL
		Type	Local
		At	Hair

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstChildEmitL.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On	Time
	Time	1

	Event
		EName	ChargeL
		Type	Local
		At	EmitterL

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCharge.part
		PMagnet	EmitterPos
	End

	Event
		EName	ChargeL01
		Type	Local
		At	ChargeL
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstChildEmitL02.bhvr
		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCharge.part
		PMagnet	EmitterPos
	End
End

################################################################################
##############################    R i g h t    #################################
################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterR
		Type	Local
		At	Hair

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstChildEmitR.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On	Time
	Time	1

	Event
		EName	ChargeR
		Type	Local
		At	EmitterR

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCharge.part
		PMagnet	EmitterPos
	End

	Event
		EName	ChargeR01
		Type	Local
		At	ChargeR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstChildEmitR02.bhvr
		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCharge.part
		PMagnet	EmitterPos
	End
End

################################################################################
############################   c e n t e r   ###################################
################################################################################

Condition
	On	Time
	Time	40

	Event
		EName	ChargeSM01
		Type	Local
		At	Emitter

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH.part
	End
End

Condition
	On	Time
	Time	45

	Event
		EName	ChargeSM01
		Type	Local
		At	Emitter

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH01.part
	End
End

################################################################################
##############################    L e f t    ###################################
################################################################################

Condition
	On	Time
	Time	40

	Event
		EName	ChargeSM01
		Type	Local
		At	ChargeL

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH.part
	End

	Event
		EName	ChargeSM01
		Type	Local
		At	ChargeL01

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH.part
	End
End

Condition
	On	Time
	Time	46

	Event
		EName	ChargeSM01
		Type	Local
		At	ChargeL

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH01.part
	End

	Event
		EName	ChargeSM01
		Type	Local
		At	ChargeL01

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH01.part
	End
End

################################################################################
##############################    R i g h t    #################################
################################################################################

Condition
	On	Time
	Time	40

	Event
		EName	ChargeSM01
		Type	Local
		At	ChargeR

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH.part
	End

	Event
		EName	ChargeSM01
		Type	Local
		At	ChargeR01

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH.part
	End
End

Condition
	On	Time
	Time	46

	Event
		EName	ChargeSM01
		Type	Local
		At	ChargeR

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH01.part
	End

	Event
		EName	ChargeSM01
		Type	Local
		At	ChargeR01

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstChargesSH01.part
	End
End

End