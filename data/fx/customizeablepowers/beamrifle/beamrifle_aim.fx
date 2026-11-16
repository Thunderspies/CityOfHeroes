#############################################################
## BeamRifle_Aim.fx
#############################################################

FxInfo
Lifespan 80
#########################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_Aim_Activate_01 100 100 .25
	End
End


Condition
	On time
	Time 0

	Event
		ENAME   WeaponOffset
		Type	Local
		At	C_Chest
		ChildFx	:Child_Aim.fx
		Lifespan 25
	End
End



Condition
	On Time
	Time 0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part	:YellowUpGlow.part
		Part	:YellowUpSpecks2.part
		#Sound	clawYellow3 100 30 .8
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End
End

Condition
	On Time
	Time 10

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part	:YellowUpSpecks.part
		Part	:YellowUpRing01.part
		Part	:YellowUpRing01a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End
End

Condition
	On Time
	Time 20

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part	:YellowUpSpecks.part
		Part	:YellowUpRing02.part
		Part	:YellowUpRing02a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End
End

Condition
	On Time
	Time 30

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part	:YellowUpSpecks2.part
		Part	:YellowUpRingSlow.part
		Part	:YellowUpRing03.part
		Part	:YellowUpRing03a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
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


#############################################################
End
