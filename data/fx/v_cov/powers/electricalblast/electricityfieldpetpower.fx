#########################################################
## Voltaic Sentinel - Attack
########################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	Hips
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArchField.part
		Part4	:ElectricityArchField2.part
		Part5	:FieldArchs1.part
		Part3	:ElectricityRingField.part
		#Sound	chargebolt 100 30 .77
		Lifespan 55
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	BurstL
		Type	local
		At	Hips
		Part4	:ElectricitySparkBurst.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	Local
		At	Hips
		ChildFx :Child_ElectricBeamCycle.fx
		Lifespan 42
	End
End

#############################################################

End