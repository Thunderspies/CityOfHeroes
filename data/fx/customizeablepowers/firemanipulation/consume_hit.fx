#############################################################
## Consume_Hit.fx
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
		Part1	CustomizeablePowers\FireManipulation\ConsumeMediumfire_flame_A.part
		Part2	CustomizeablePowers\FireManipulation\ConsumeMediumfire_flame_B.part
		Part4	CustomizeablePowers\FireManipulation\spark1.part
		Sound consumehit 50 50 0.44
		LifeSpan 14
	End
#
#	Event
#		EName 	Fire
#		Type	Posit
#		At	chest
#		Part1	CustomizeablePowers\FireManipulation\InfernoCare.part
#		Part2	CustomizeablePowers\FireManipulation\InfernoCare2.part
#		LifeSpan 6
#	End
End

##############################################################
#
#Condition
#	On	Time
#	Time	8.5
#
#	Event
#		EName 	Prime
#		Type	Start
#		At	chest
#		Bhvr	CustomizeablePowers\FireManipulation\ConsumeProjectile.bhvr
#		Part1	CustomizeablePowers\FireManipulation\EndurenceFlame.part
#		Part2	CustomizeablePowers\FireManipulation\EndurenceFlame2.part
#		Part4	CustomizeablePowers\FireManipulation\Spark.part
#		Magnet	T_Hips
#		LookAt	T_Hips
#	End
#End
#
##############################################################
#
#Condition
#	On	PrimeHit
#
#	Event
#		EName 	Prime
#		Type	Destroy
#	End
#End

#############################################################

End