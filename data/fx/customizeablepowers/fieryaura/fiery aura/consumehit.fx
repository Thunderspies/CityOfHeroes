#############################################################
## ConsumeHit.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	local
		At	chest
		Part1	CustomizeablePowers\FieryAura\ConsumeMediumfire_flame_A.part
		Part2	CustomizeablePowers\FieryAura\ConsumeMediumfire_flame_B.part
		Part4	CustomizeablePowers\FieryAura\spark1.part
		Sound consumehit 50 50 .44
		LifeSpan 14
	End

	Event
		EName 	Fire
		Type	Posit
		At	chest
		Part1	CustomizeablePowers\FieryAura\InfernoCare.part
		Part2	CustomizeablePowers\FieryAura\InfernoCare2.part
		LifeSpan 6
	End
End

Condition
	On	Time
	Time	8.5

	Event
		EName 	Prime
		Type	Start
		At	chest
		Bhvr	CustomizeablePowers\FieryAura\ConsumeProjectile.bhvr
		Magnet	T_Hips
		LookAt	T_Hips
		Part1	CustomizeablePowers\FieryAura\EndurenceFlame.part
		Part2	CustomizeablePowers\FieryAura\EndurenceFlame2.part
		Part4	CustomizeablePowers\FieryAura\Spark.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End