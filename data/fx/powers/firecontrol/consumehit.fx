#############################################################
## ConsumeHit.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	local
		At	chest
		Part1	:ConsumeMediumfire_flame_A.part
		Part2	:ConsumeMediumfire_flame_B.part
		Part4	WORLD\Fire\spark1.part
		Sound consumehit 50 50 .44
		LifeSpan 14
	End

	Event
		EName 	Fire
		Type	Posit
		At	chest
		Part1	:InfernoCare.part
		Part2	:InfernoCare2.part
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
		Bhvr	behaviors\ConsumeProjectile.bhvr
		Magnet	T_Hips
		LookAt	T_Hips
		Part1	:EndurenceFlame.part
		Part2	:EndurenceFlame2.part
		Part4	:Spark.part
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