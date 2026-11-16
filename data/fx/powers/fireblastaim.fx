#############################################################
## FireBlastAim.fx
#############################################################

FxInfo

#############################################################

Condition
	On Time
	Time 0

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:ChestFire.part

		LifeSpan	20
	End

#############################################################

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:HealFire.part
		LifeSpan	30

	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:HealFire.part
		LifeSpan	30

	End


	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	SpadR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	SpadL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	Hips
		Part	:HealFirehips.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:FireBuildupSparks.part
		LifeSpan	30
	End
End

Condition
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound aim2 60 60 .6
		End
end




Condition
	On Time
	Time 0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	:YellowUpGlow.part
		Part2	:YellowUpSpecks2.part
		#Sound	clawYellow3 100 30 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End

Condition
	On Time
	Time 10

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks.part
		Part2	:YellowUpRing01.part
		Part3	:YellowUpRing01a.part
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
		#Part1	:YellowUpSpecks.part
		Part3	:YellowUpRing02.part
		Part4	:YellowUpRing02a.part
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
		#Part1	:YellowUpSpecks2.part
		Part2	:YellowUpRingSlow.part
		Part3	:YellowUpRing03.part
		Part4	:YellowUpRing03a.part
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

