#
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_PhaseAura_Loop 100 100 .4
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type PositOnly
			EName UArmLFX
			POther Neck
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At Chest
			Type PositOnly
			EName UArmLFX
			POther Hips
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At UArmL
			Type PositOnly
			EName UArmLFX
			POther LarmL
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At UArmR
			Type PositOnly
			EName UArmRFX
			POther LArmR
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At LArmL
			Type PositOnly
			EName LArmLFX
			POther HandL
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At LArmR
			Type PositOnly
			EName LArmRFX
			POther HandR
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At ULegL
			Type PositOnly
			EName ULegLFX
			POther LLegL
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At ULegR
			Type PositOnly
			EName ULegRFX
			POther LLegR
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At LLegL
			Type PositOnly
			EName FootLFX
			POther FootL
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

		Event
			At LLegR
			Type PositOnly
			EName FootRFX
			POther FootR
			Part ENEMYFX\Praetorian\MoM\PhaseMist_01.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_02.part
			Part ENEMYFX\Praetorian\MoM\PhaseGlow.part
			Part ENEMYFX\Praetorian\MoM\PhaseMist_03.part
		End

	End

End
