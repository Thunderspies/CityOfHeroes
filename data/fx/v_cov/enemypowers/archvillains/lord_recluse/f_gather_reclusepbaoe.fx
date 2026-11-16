
FxInfo
	
#LifeSpan	85


Condition
	On	Time
	Time	0

	Event
		EName	BallCore
		Type	Local
		At	Root
		bhvrOverride
			PositionOffset	.1 -1.5 1.2
		End

		bhvr	:B_Scale_Growing.bhvr
#		Geom	Testing_TargetB
	End
End

#########  Emitter Pos BackPack Tips Right

Condition
	On	Time
	Time	0

	Event
		EName	R1_End
		Type	Local
		At	C_Fore_ToeR
		bhvrOverride
			PositionOffset	0 2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	R2_End
		Type	Local
		At	C_ToeR
		bhvrOverride
			PositionOffset	0 2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R3_End
		Type	Local
		At	C_RingR
		bhvrOverride
			PositionOffset	2.2 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R4_End
		Type	Local
		At	C_Hind_ToeR
		bhvrOverride
			PositionOffset	0 -2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

#########  Emitter Pos BackPack Tips Left

	Event
		EName	L1_End
		Type	Local
		At	C_Fore_ToeL
		bhvrOverride
			PositionOffset	0 2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	L2_End
		Type	Local
		At	C_ToeL
		bhvrOverride
			PositionOffset	0 2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L3_End
		Type	Local
		At	C_RingL
		bhvrOverride
			PositionOffset	2.2 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L4_End
		Type	Local
		At	C_Hind_ToeL
		bhvrOverride
			PositionOffset	0 -2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

End


################################################################################
############################   build up wrists   ###############################
################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	ChargeR
		Type	Local
		At	HandR

		Part1	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Charge_SparkMagnet.part
		Part2	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Charge_SparkMagnetLight.part
		PMagnet	HandR
	End

	Event
		EName	ChargeL
		Type	Local
		At	HandL

		Part1	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Charge_SparkMagnet.part
		Part2	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Charge_SparkMagnetLight.part
		PMagnet	HandL
	End
End

End

