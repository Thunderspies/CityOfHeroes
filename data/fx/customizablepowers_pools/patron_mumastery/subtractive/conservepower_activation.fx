#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

#########################################################

Condition
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound elecbuildup2b 60 60 .7
		End
end



Condition
	On Time
	Time 0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	POWERS\BuildUpGlow.part
		Part2	POWERS\BuildUpSpecks2.part
		#Sound	clawBuild3 100 30 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part	:Red_ElectricityArchLarge2WhiteA.part
		Part	:Red_ElectricityArchLarge2A.part
		Part	:Red_ElectricityArchLargeA.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	37
	End

End

Condition
	On Time
	Time 10

	Event
		EName	Pow
		Type 	local
		At	Chest
		Part2	POWERS\BuildUpRing01.part
		Part3	POWERS\BuildUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 20

	Event
		EName	Pow
		Type 	local
		At	Chest
		Part3	POWERS\BuildUpRing02.part
		Part4	POWERS\BuildUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 30

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:BuildUpSpecks2.part
		Part2	POWERS\BuildUpRingSlow.part
		Part3	POWERS\BuildUpRing03.part
		Part4	POWERS\BuildUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End


Condition
	On Time
	Time 13

	Event
		EName	pow2
		Type 	Destroy

	End

End

Condition
	On Time
	Time 75

	Event
		EName	all
		Type 	Destroy

	End

End



End

