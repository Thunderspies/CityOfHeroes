#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	180


###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EnergyBalls
		Type	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr

		Pmagnet	Chest
#		Part	:EnergyBall.part
#		Part	:SolidEnergyBall.part
#		Part	:DarkEnergyBall.part
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Ename	Ring1
		Type	Local
		At	Head
		Part1	:EnergyShockRing.part
		bhvr	Behaviors/SpinHit2.bhvr
	End

End

Condition
	On 	Time
	Time 	12

	Event
		Ename	Ring1
		Type	Local
		At	Head
		Part1	:EnergyShockRing.part
		bhvr	Behaviors/SpinHit1.bhvr
	End


End

Condition
	On 	Time
	Time 	17

	Event
		Ename	Ring2
		Type	Local
		At	Head
		bhvr	Behaviors/SpinHit3.bhvr
		Part1	:EnergyShockRing.part
	End

End



